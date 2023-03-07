//
// DeviceResources.cpp - A wrapper for the Direct3D 11 device and swapchain
//                       (requires DirectX 11.1 Runtime)
//

#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"

using namespace DirectX;
using namespace DX;

using Microsoft::WRL::ComPtr;

namespace
{
#if defined(_DEBUG)
    // Check for SDK Layer support.
    inline bool SdkLayersAvailable()
    {
        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
            nullptr,                    // Any feature level will do.
            0,
            D3D11_SDK_VERSION,
            nullptr,                    // No need to keep the D3D device reference.
            nullptr,                    // No need to know the feature level.
            nullptr                     // No need to keep the D3D device context reference.
            );

        return SUCCEEDED(hr);
    }
#endif

    inline DXGI_FORMAT NoSRGB(DXGI_FORMAT fmt)
    {
        switch (fmt)
        {
        case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   return DXGI_FORMAT_R8G8B8A8_UNORM;
        case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8A8_UNORM;
        case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:   return DXGI_FORMAT_B8G8R8X8_UNORM;
        default:                                return fmt;
        }
    }
};

// Constructor for DeviceResources.
DeviceResources::DeviceResources(
    DXGI_FORMAT backBufferFormat,
    DXGI_FORMAT depthBufferFormat,
    UINT backBufferCount,
    D3D_FEATURE_LEVEL minFeatureLevel,
    unsigned int flags) noexcept :
        m_screenViewport{},
        m_backBufferFormat(backBufferFormat),
        m_depthBufferFormat(depthBufferFormat),
        m_backBufferCount(backBufferCount),
        m_d3dMinFeatureLevel(minFeatureLevel),
        m_window(nullptr),
        m_d3dFeatureLevel(D3D_FEATURE_LEVEL_9_1),
        m_outputSize{0, 0, 1, 1},
        m_colorSpace(DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709),
        m_options(flags | c_FlipPresent),
        m_deviceNotify(nullptr)
{
}

// Configures the Direct3D device, and stores handles to it and the device context.
BOOL DeviceResources::CreateDeviceResources()
{
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    if (SdkLayersAvailable())
    {
        // If the project is in a debug build, enable debugging via SDK Layers with this flag.
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    }
    else
    {
		DEBUG_MESSAGE("WARNING: Direct3D Debug Device is not available\n");
    }
#endif

    CreateFactory();

    // Determines whether tearing support is available for fullscreen borderless windows.
    if (m_options & c_AllowTearing)
    {
        BOOL allowTearing = FALSE;

        ComPtr<IDXGIFactory5> factory5;
        HRESULT hr = m_dxgiFactory.As(&factory5);
        if (SUCCEEDED(hr))
        {
            hr = factory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
        }

        if (FAILED(hr) || !allowTearing)
        {
            m_options &= ~c_AllowTearing;
			DEBUG_MESSAGE("WARNING: Variable refresh rate displays not supported.\n");
        }
    }

    // Disable HDR if we are on an OS that can't support FLIP swap effects
    if (m_options & c_EnableHDR)
    {
        ComPtr<IDXGIFactory5> factory5;
        if (FAILED(m_dxgiFactory.As(&factory5)))
        {
            m_options &= ~c_EnableHDR;
			DEBUG_MESSAGE("WARNING: HDR swap chains not supported.\n");
        }
    }

    // Disable FLIP if not on a supporting OS
    if (m_options & c_FlipPresent)
    {
        ComPtr<IDXGIFactory4> factory4;
        if (FAILED(m_dxgiFactory.As(&factory4)))
        {
            m_options &= ~c_FlipPresent;
			DEBUG_MESSAGE("INFO: Flip swap effects not supported.\n");
        }
    }

    // Determine DirectX hardware feature levels this app will support.
    static const D3D_FEATURE_LEVEL s_featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    UINT featLevelCount = 0;
    for (; featLevelCount < _countof(s_featureLevels); ++featLevelCount)
    {
        if (s_featureLevels[featLevelCount] < m_d3dMinFeatureLevel)
            break;
    }

	ASSERT( featLevelCount != 0, "minFeatureLevel too high.\n" );

    ComPtr<IDXGIAdapter1> adapter;
    GetHardwareAdapter(adapter.GetAddressOf());

    // Create the Direct3D 11 API device object and a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    HRESULT hr = E_FAIL;
    if (adapter)
    {
        hr = D3D11CreateDevice(
            NULL,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            s_featureLevels,
            featLevelCount,
            D3D11_SDK_VERSION,
            device.GetAddressOf(),  // Returns the Direct3D device created.
            &m_d3dFeatureLevel,     // Returns feature level of device created.
            context.GetAddressOf()  // Returns the device immediate context.
            );
    }
#if !defined(_DEBUG)
    else
    {
        io::OutputMessage( "No Direct3D hardware device found.\n" );
		return TRUE;
    }
#else
    if (FAILED(hr))
    {
        // If the initialization fails, fall back to the WARP device.
        // For more information on WARP, see:
        // http://go.microsoft.com/fwlink/?LinkId=286690
        hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_WARP, // Create a WARP device instead of a hardware device.
            nullptr,
            creationFlags,
            s_featureLevels,
            featLevelCount,
            D3D11_SDK_VERSION,
            device.GetAddressOf(),
            &m_d3dFeatureLevel,
            context.GetAddressOf()
            );

        if (SUCCEEDED(hr))
        {
			io::OutputMessage("Direct3D Adapter - WARP\n");
        }
		else
		{
			io::OutputMessage( "Failure Creating D3D WARP device.\n" );
			return TRUE;
		}
    }
#endif

#ifdef _DEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

	D3D11_RASTERIZER_DESC rasteriserDesc = D3D11_RASTERIZER_DESC {
		D3D11_FILL_SOLID,
		D3D11_CULL_NONE,
		FALSE,
		0,
		0.0f,
		0.0f,
		TRUE,
		FALSE,
		FALSE,
		FALSE
	};
	ID3D11RasterizerState* rasteriserState = nullptr;
	hr = device->CreateRasterizerState( &rasteriserDesc, &rasteriserState );
	ASSERT_HANDLE( hr );
	if( SUCCEEDED( hr ) )
	{
		context->RSSetState( rasteriserState );
	}

	device.As(&m_d3dDevice);
	ASSERT( m_d3dDevice != nullptr, "Unable to get D3D device.\n" );
	context.As(&m_d3dContext);
	ASSERT( m_d3dContext != nullptr, "Unable to get D3D context.\n" );
	context.As(&m_d3dAnnotation);
	ASSERT( m_d3dAnnotation != nullptr, "Unable to get D3D annotation.\n" );

	return FALSE;
}

// These resources need to be recreated every time the window size is changed.
BOOL DeviceResources::CreateWindowSizeDependentResources()
{
	HRESULT hr = 0;

    if (!m_window)
    {
        ASSERT( FALSE, "Call SetWindow with a valid Win32 window handle.\n" );
		return TRUE;
    }

    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews[] = {nullptr};
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_d3dRenderTargetView.Reset();
    m_d3dDepthStencilView.Reset();
    m_renderTarget.Reset();
    m_depthStencil.Reset();
    m_d3dContext->Flush();

    // Determine the render target size in pixels.
    UINT backBufferWidth = std::max<UINT>(static_cast<UINT>(m_outputSize.right - m_outputSize.left), 1u);
    UINT backBufferHeight = std::max<UINT>(static_cast<UINT>(m_outputSize.bottom - m_outputSize.top), 1u);
    DXGI_FORMAT backBufferFormat = (m_options & (c_FlipPresent | c_AllowTearing | c_EnableHDR)) ? NoSRGB(m_backBufferFormat) : m_backBufferFormat;

    if (m_swapChain)
    {
        // If the swap chain already exists, resize it.
        hr = m_swapChain->ResizeBuffers(
            m_backBufferCount,
            backBufferWidth,
            backBufferHeight,
            backBufferFormat,
            (m_options & c_AllowTearing) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u
            );

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
			DEBUG_MESSAGE("Device Lost on ResizeBuffers: Reason code 0x%08X\n", (hr == DXGI_ERROR_DEVICE_REMOVED) ? m_d3dDevice->GetDeviceRemovedReason() : hr);
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            HandleDeviceLost();

            // Everything is set up now. Do not continue execution of this method. HandleDeviceLost will reenter this method
            // and correctly set up the new device.
            return TRUE;
        }
        else
        {
			ASSERT_HANDLE(hr);
        }
    }
	else
	{
		ASSERT( m_d3dDevice.Get(), "Invalid D3D device.\n" );
		IDXGISwapChain* swapchain = nullptr;

#ifdef ENABLE_PIX
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferDesc.Width = backBufferWidth;
		swapChainDesc.BufferDesc.Height = backBufferHeight;
		swapChainDesc.BufferDesc.Format = backBufferFormat;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = m_backBufferCount;
		swapChainDesc.OutputWindow = m_window;
		swapChainDesc.Windowed = TRUE;
		swapChainDesc.SwapEffect = ( m_options & ( c_FlipPresent | c_AllowTearing | c_EnableHDR ) ) ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = ( m_options & c_AllowTearing ) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u;
		hr = m_dxgiFactory->CreateSwapChain(
			m_d3dDevice.Get(),
			&swapChainDesc,
			&swapchain
		);

		m_swapChain = swapchain;
#else
		// Create a descriptor for the swap chain.
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.Width = backBufferWidth;
		swapChainDesc.Height = backBufferHeight;
		swapChainDesc.Format = backBufferFormat;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = m_backBufferCount;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.SwapEffect = ( m_options & ( c_FlipPresent | c_AllowTearing | c_EnableHDR ) ) ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapChainDesc.Flags = ( m_options & c_AllowTearing ) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
		fsSwapChainDesc.Windowed = TRUE;

		// Create a SwapChain from a Win32 window.
		hr = m_dxgiFactory->CreateSwapChainForHwnd(
			m_d3dDevice.Get(),
			m_window,
			&swapChainDesc,
			&fsSwapChainDesc,
			nullptr, m_swapChain.ReleaseAndGetAddressOf()
		);
#endif // ENABLE_PIX
		ASSERT( SUCCEEDED( hr ), "Unable to create swap chain.\n" );

        // This class does not support exclusive full-screen mode and prevents DXGI from responding to the ALT+ENTER shortcut
        hr = m_dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER);
		ASSERT( SUCCEEDED( hr ), "Unable to make window assocciation.\n" );
    }

    // Handle color space settings for HDR
    UpdateColorSpace();

    // Create a render target view of the swap chain back buffer.
    hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(m_renderTarget.ReleaseAndGetAddressOf()));
	ASSERT( SUCCEEDED( hr ), "Can't get swap chain buffer.\n" );

    CD3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc(D3D11_RTV_DIMENSION_TEXTURE2D, m_backBufferFormat);
    hr = m_d3dDevice->CreateRenderTargetView(
        m_renderTarget.Get(),
        &renderTargetViewDesc,
        m_d3dRenderTargetView.ReleaseAndGetAddressOf()
        );
	ASSERT( SUCCEEDED( hr ), "Can't create render target.\n" );

    if (m_depthBufferFormat != DXGI_FORMAT_UNKNOWN)
    {
        // Create a depth stencil view for use with 3D rendering if needed.
        CD3D11_TEXTURE2D_DESC depthStencilDesc(
            m_depthBufferFormat,
            backBufferWidth,
            backBufferHeight,
            1, // This depth stencil view has only one texture.
            1, // Use a single mipmap level.
            D3D11_BIND_DEPTH_STENCIL
            );

        hr = m_d3dDevice->CreateTexture2D(
            &depthStencilDesc,
            nullptr,
            m_depthStencil.ReleaseAndGetAddressOf()
            );
		ASSERT( SUCCEEDED( hr ), "Can't create render target.\n" );

        CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
        hr = m_d3dDevice->CreateDepthStencilView(
            m_depthStencil.Get(),
            &depthStencilViewDesc,
            m_d3dDepthStencilView.ReleaseAndGetAddressOf()
            );
		ASSERT( SUCCEEDED( hr ), "Can't create stencil view.\n" );

    }

    // Set the 3D rendering viewport to target the entire window.
    m_screenViewport = CD3D11_VIEWPORT(
        0.0f,
        0.0f,
        static_cast<float>(backBufferWidth),
        static_cast<float>(backBufferHeight)
        );

	return FALSE;
}

// This method is called when the Win32 window is created (or re-created).
void DeviceResources::SetWindow(HWND window, int width, int height)
{
    m_window = window;

    m_outputSize.left = m_outputSize.top = 0;
    m_outputSize.right = width;
    m_outputSize.bottom = height;
}

// This method is called when the Win32 window changes size
bool DeviceResources::WindowSizeChanged(int width, int height)
{
    RECT newRc;
    newRc.left = newRc.top = 0;
    newRc.right = width;
    newRc.bottom = height;
    if (newRc == m_outputSize)
    {
        // Handle color space settings for HDR
        UpdateColorSpace();

        return false;
    }

    m_outputSize = newRc;
    CreateWindowSizeDependentResources();
    return true;
}

// Recreate all device resources and set them back to the current state.
void DeviceResources::HandleDeviceLost()
{
    if (m_deviceNotify)
    {
        m_deviceNotify->OnDeviceLost();
    }

    m_d3dDepthStencilView.Reset();
    m_d3dRenderTargetView.Reset();
    m_renderTarget.Reset();
    m_depthStencil.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dAnnotation.Reset();

#ifdef _DEBUG
    {
        ComPtr<ID3D11Debug> d3dDebug;
        if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
        {
            d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY);
        }
    }
#endif

    m_d3dDevice.Reset();
    m_dxgiFactory.Reset();

    CreateDeviceResources();
    CreateWindowSizeDependentResources();

    if (m_deviceNotify)
    {
        m_deviceNotify->OnDeviceRestored();
    }
}

// Present the contents of the swap chain to the screen.
void DeviceResources::Present()
{
    HRESULT hr;
    if (m_options & c_AllowTearing)
    {
        // Recommended to always use tearing if supported when using a sync interval of 0.
        hr = m_swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
    }
    else
    {
        // The first argument instructs DXGI to block until VSync, putting the application
        // to sleep until the next VSync. This ensures we don't waste any cycles rendering
        // frames that will never be displayed to the screen.
        hr = m_swapChain->Present(1, 0);
    }

    // Discard the contents of the render target.
    // This is a valid operation only when the existing contents will be entirely
    // overwritten. If dirty or scroll rects are used, this call should be removed.
    m_d3dContext->DiscardView(m_d3dRenderTargetView.Get());

    if (m_d3dDepthStencilView)
    {
        // Discard the contents of the depth stencil.
        m_d3dContext->DiscardView(m_d3dDepthStencilView.Get());
    }

    // If the device was removed either by a disconnection or a driver upgrade, we
    // must recreate all device resources.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
		DEBUG_MESSAGE("Device Lost on Present: Reason code 0x%08X\n", (hr == DXGI_ERROR_DEVICE_REMOVED) ? m_d3dDevice->GetDeviceRemovedReason() : hr);
        HandleDeviceLost();
    }
    else
    {
        ASSERT_HANDLE(hr);

        if (!m_dxgiFactory->IsCurrent())
        {
            // Output information is cached on the DXGI Factory. If it is stale we need to create a new factory.
            CreateFactory();
        }
    }
}

void DeviceResources::CreateFactory()
{
	HRESULT hr = 0;
#if defined(_DEBUG) && (_WIN32_WINNT >= 0x0603 /*_WIN32_WINNT_WINBLUE*/)
    bool debugDXGI = false;
    {
        ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
        {
            debugDXGI = true;

            hr = (CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));
			ASSERT( SUCCEEDED( hr ), "Can't create GXGI factory.\n" );

            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

            DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
            {
                80 /* IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides. */,
            };
            DXGI_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
        }
    }

    if (!debugDXGI)
#endif

    hr = CreateDXGIFactory1(IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf()));
	ASSERT_HANDLE( hr );
}

// This method acquires the first available hardware adapter.
// If no such adapter can be found, *ppAdapter will be set to nullptr.
void DeviceResources::GetHardwareAdapter(IDXGIAdapter1** ppAdapter)
{
	HRESULT hr = 0;
    *ppAdapter = nullptr;

    ComPtr<IDXGIAdapter1> adapter;

#if defined(__dxgi1_6_h__) && defined(NTDDI_WIN10_RS4)
    ComPtr<IDXGIFactory6> factory6;
    hr = m_dxgiFactory.As(&factory6);
    if (SUCCEEDED(hr))
    {
        for (UINT adapterIndex = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf())));
            adapterIndex++)
        {
            DXGI_ADAPTER_DESC1 desc;
            hr = adapter->GetDesc1(&desc);
			ASSERT( SUCCEEDED( hr ), "Can't get adapter desc.\n" );

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                continue;
            }

			DEBUG_MESSAGE("Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);

            break;
        }
    }
#endif
    if (!adapter)
    {
        for (UINT adapterIndex = 0;
            SUCCEEDED(m_dxgiFactory->EnumAdapters1(
                adapterIndex,
                adapter.ReleaseAndGetAddressOf()));
            adapterIndex++)
        {
            DXGI_ADAPTER_DESC1 desc;
            hr = adapter->GetDesc1(&desc);
			ASSERT( SUCCEEDED( hr ), "Can't get adapter desc.\n" );

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                continue;
            }

			DEBUG_MESSAGE("Direct3D Adapter (%u): VID:%04X, PID:%04X - %ls\n", adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);

            break;
        }
    }

    *ppAdapter = adapter.Detach();
}

// Sets the color space for the swap chain in order to handle HDR output.
void DeviceResources::UpdateColorSpace()
{
	HRESULT hr = 0;
    DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

    bool isDisplayHDR10 = false;

#if defined(NTDDI_WIN10_RS2)
    if (m_swapChain)
    {
        ComPtr<IDXGIOutput> output;
        if (SUCCEEDED(m_swapChain->GetContainingOutput(output.GetAddressOf())))
        {
            ComPtr<IDXGIOutput6> output6;
            if (SUCCEEDED(output.As(&output6)))
            {
                DXGI_OUTPUT_DESC1 desc;
                hr = output6->GetDesc1(&desc);
				ASSERT( SUCCEEDED( hr ), "Can't get adapter desc.\n" );

                if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
                {
                    // Display output is HDR10.
                    isDisplayHDR10 = true;
                }
            }
        }
    }
#endif

    if ((m_options & c_EnableHDR) && isDisplayHDR10)
    {
        switch (m_backBufferFormat)
        {
        case DXGI_FORMAT_R10G10B10A2_UNORM:
            // The application creates the HDR10 signal.
            colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
            break;

        case DXGI_FORMAT_R16G16B16A16_FLOAT:
            // The system creates the HDR10 signal; application uses linear values.
            colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
            break;

        default:
            break;
        }
    }

    m_colorSpace = colorSpace;

    ComPtr<IDXGISwapChain3> swapChain3;
    if (SUCCEEDED(m_swapChain.As(&swapChain3)))
    {
        UINT colorSpaceSupport = 0;
        if (SUCCEEDED(swapChain3->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport))
            && (colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
        {
            hr = swapChain3->SetColorSpace1(colorSpace);
			ASSERT( SUCCEEDED( hr ), "Can't set color space.\n" );
        }
    }
}
