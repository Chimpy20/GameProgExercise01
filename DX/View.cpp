#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "DX\View.h"

using namespace DirectX;

namespace DX
{

View::View( DeviceResources* deviceResources ) :
	m_deviceResources( deviceResources ),
	m_constantBuffer( nullptr )
{
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

	CD3D11_BUFFER_DESC bufferDesc( sizeof( ConstantBuffer ), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE );
	device->CreateBuffer( &bufferDesc, nullptr, &m_constantBuffer );
	ASSERT( m_constantBuffer != nullptr, "Unable to create constant buffer.\n" );

	// Initialize the world matrix
	XMStoreFloat4x4( &m_worldMatrix, DirectX::XMMatrixIdentity() );

	// Initialize the view matrix
	static const DirectX::XMVECTORF32 eye = { 0.0f, 4.0f, -10.0f, 0.0f };
	static const DirectX::XMVECTORF32 at = { 0.0f, 0.0f, 0.0f, 0.0f };
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0 };
	XMStoreFloat4x4( &m_viewMatrix, XMMatrixLookAtLH( eye, at, up ) );

	// Initialize the projection matrix
	auto size = m_deviceResources->GetOutputSize();
	XMMATRIX projection = XMMatrixPerspectiveFovLH( XM_PIDIV4, float( size.right ) / float( size.bottom ), 0.01f, 100.0f );
	XMStoreFloat4x4( &m_projectionMatrix, projection );
}

void View::Refresh()
{
	// Set the per-frame constants
	ConstantBuffer sceneParameters = {};

	// Shaders compiled with default row-major matrices
	sceneParameters.worldMatrix = XMMatrixTranspose( XMLoadFloat4x4( &m_worldMatrix ) );
	sceneParameters.viewMatrix = XMMatrixTranspose( XMLoadFloat4x4( &m_viewMatrix ) );
	sceneParameters.projectionMatrix = XMMatrixTranspose( XMLoadFloat4x4( &m_projectionMatrix ) );

	ASSERT( m_deviceResources != nullptr, "Device resources doesn't exist.\n" );
	ID3D11DeviceContext* const deviceContext = m_deviceResources->GetD3DDeviceContext();

	ASSERT( m_constantBuffer != nullptr, "Constant buffer doesn't exist. Has View::Initialise() been called?\n" );

	D3D11_MAPPED_SUBRESOURCE mapped;
	HRESULT hr = deviceContext->Map( m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped );
	ASSERT_HANDLE( hr );
	memcpy( mapped.pData, &m_constantBuffer, sizeof( ConstantBuffer ) );
	deviceContext->Unmap( m_constantBuffer, 0 );

	// Vertex shader needs view and projection matrices to perform vertex transform
	deviceContext->VSSetConstantBuffers( 0, 1, &m_constantBuffer );
}

void View::Shutdown()
{
}

} // namespace DX
