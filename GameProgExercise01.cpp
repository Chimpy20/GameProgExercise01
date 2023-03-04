#include "GameProgExercise01.h"
#include "Core.h"
#include "Containers\List.h"
#include "DX\Input.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//using Microsoft::WRL::ComPtr;

LPCWSTR g_szAppName = L"GameProgExercise01";
static const float MinFrameTime = 1.0f / 75.0f - 0.0001f;

LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );

// Entry point
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	bool error = false;

	memory::Heap::Create();

	// This needs to be done after the heap is created
	utils::file::InitialiseFileSystem();

	if( !XMVerifyCPUSupport() )
		return 1;

	HRESULT hr = CoInitializeEx( nullptr, COINITBASE_MULTITHREADED );
	if( FAILED( hr ) )
		return 1;

	DEBUG_MESSAGE( "Creating core object.\n" );
	Core* const core = new Core();

	// Register class and create window
	{
		// Register class
		WNDCLASSEXW wcEx;
		wcEx.cbSize = sizeof( WNDCLASSEXW );
		wcEx.style = CS_HREDRAW | CS_VREDRAW;
		wcEx.lpfnWndProc = WndProc;
		wcEx.cbClsExtra = 0;
		wcEx.cbWndExtra = 0;
		wcEx.hInstance = hInstance;
		wcEx.hIcon = LoadIconW( hInstance, L"IDI_ICON" );
		wcEx.hCursor = LoadCursorW( nullptr, IDC_ARROW );
		wcEx.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
		wcEx.lpszMenuName = nullptr;
		wcEx.lpszClassName = L"DirectXTKSimpleSampleWindowClass";
		wcEx.hIconSm = LoadIconW( wcEx.hInstance, L"IDI_ICON" );
		if( !RegisterClassExW( &wcEx ) )
			return 1;

		// Create window
		int width = 1280, height = 720;

		RECT rc = { 0, 0, static_cast<LONG>( width ), static_cast<LONG>( height ) };

		AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

		HWND hWnd = CreateWindowExW( 0, L"DirectXTKSimpleSampleWindowClass", g_szAppName, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
			nullptr );

		if( !hWnd )
			error = true;

		if( !error )
		{
			ShowWindow( hWnd, nCmdShow );

			SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( core ) );

			GetClientRect( hWnd, &rc );

			if( core->Initialise( hWnd, rc.right - rc.left, rc.bottom - rc.top ) )
			{
				error = true;
			}
		}
	}

	utils::Timers::InitialiseTimers();

	MSG msg = {};
	if( !error )
	{
		// Main message loop
		while( WM_QUIT != msg.message )
		{
			if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				utils::Timers::UpdateFrameTimer();
				const float frameTime = utils::Timers::GetFrameTime();
				if( frameTime > MinFrameTime )
					core->Update();
				core->Render();
			}
		}
	}
	else
	{
		msg.wParam = 1;
	}

	if( !error )
	{
		core->Shutdown();
	}

	delete core;

	utils::file::ShutdownFileSystem();

	CoUninitialize();

	memory::Heap::Destroy();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	//PAINTSTRUCT ps;
	//HDC hdc;

	Core* const core = Core::Get();
	DX::Input* const input = core->GetInput();

	switch( message )
	{
		case WM_CREATE:
			break;

		case WM_CLOSE:
			DestroyWindow( hWnd );
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_KEYDOWN:
		case WM_KEYUP:
			input->HandleSystemMessage( message, wParam );
			break;

		default:
			break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}
