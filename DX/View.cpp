#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "DX\View.h"

using namespace DirectX;

namespace DX
{

View::View( DeviceResources* deviceResources ) :
	m_deviceResources( deviceResources ),
	m_vpConstantBuffer( nullptr )
{
	// Initialize the world matrix
	m_viewMatrix = XMMatrixIdentity();
	m_projectionMatrix = XMMatrixIdentity();

	m_viewPosition = XMVectorZero();
	m_viewDirection = XMVECTOR{ -1.0f, 1.0f, 1.0f, 0.0f };
	m_viewDirection = XMVector3Normalize( m_viewDirection );

	m_ambient = XMFLOAT4{ 0.2f, 0.2f, 0.4f, 0.0f };
	m_dirLightDir = XMVECTOR{ 0.25f, -0.75f, -0.25f, 0.0f };
	m_dirLightDir = XMVector3Normalize( m_dirLightDir );
	m_dirLightCol = XMFLOAT4{ 0.8f, 0.8f, 0.6f, 0.0f };

}

View::~View()
{
}

void View::Initialise()
{
	ASSERT( m_deviceResources != nullptr, "Device resources doesn't exist.\n" );
	ID3D11DeviceContext* const deviceContext = m_deviceResources->GetD3DDeviceContext();

	ID3D11Device* device = nullptr;
	deviceContext->GetDevice( &device );
	ASSERT( device != nullptr, "Unable to get D3D device.\n" );

	CD3D11_BUFFER_DESC bufferDesc( sizeof( VPConstantBuffer ), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE );
	device->CreateBuffer( &bufferDesc, nullptr, &m_vpConstantBuffer );
	ASSERT( m_vpConstantBuffer != nullptr, "Unable to create constant buffer.\n" );

	// Initialize the projection matrix
	auto size = m_deviceResources->GetOutputSize();
	XMMATRIX projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, float( size.right ) / float( size.bottom ), 0.01f, 100.0f );
	m_projectionMatrix = projection;	
}

void View::Refresh()
{
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0 };

	m_viewPosition.m128_f32[ 3 ] = 0.0f;
	m_viewDirection.m128_f32[ 3 ] = 0.0f;
	m_viewMatrix = XMMatrixLookToLH( m_viewPosition, m_viewDirection, up );

	ASSERT( m_deviceResources != nullptr, "Device resources doesn't exist.\n" );
	ID3D11DeviceContext* const deviceContext = m_deviceResources->GetD3DDeviceContext();

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT hr = S_OK;

	// Set the per-frame constants
	VPConstantBuffer vpConstants = {};

	XMMATRIX viewProjection = XMMatrixMultiply( m_viewMatrix, m_projectionMatrix );

	// Shaders compiled with default row-major matrices
	vpConstants.viewProjectionMatrix = XMMatrixTranspose( viewProjection );
	vpConstants.vAmbient = m_ambient;
	XMStoreFloat4( &vpConstants.vDirLight0, m_dirLightDir );
	vpConstants.vDirLight0Col = m_dirLightCol;

	ASSERT( m_vpConstantBuffer != nullptr, "Constant buffer doesn't exist. Has View::Initialise() been called?\n" );

	hr = deviceContext->Map( m_vpConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped );
	ASSERT_HANDLE( hr );
	memcpy( mapped.pData, &vpConstants, sizeof( VPConstantBuffer ) );
	deviceContext->Unmap( m_vpConstantBuffer, 0 );

	deviceContext->VSSetConstantBuffers( 0, 1, &m_vpConstantBuffer );
	deviceContext->PSSetConstantBuffers( 0, 1, &m_vpConstantBuffer );
}

void View::Shutdown()
{
	if( m_vpConstantBuffer != nullptr )
		m_vpConstantBuffer->Release();
}

} // namespace DX
