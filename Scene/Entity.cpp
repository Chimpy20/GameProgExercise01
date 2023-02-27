#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Scene.h"
#include "Entity.h"

using namespace DirectX;

namespace scene
{

Entity::Entity() :
	m_inputLayout( nullptr ),
	m_vertexShader( nullptr ),
	m_pixelShader( nullptr ),
	m_vertexBuffer( nullptr ),
	m_constantBuffer( nullptr )
{
	m_orientation = XMMatrixIdentity();
	m_position.v = XMVectorZero();
	m_position.f[ 3 ] = 1.0f;
}

Entity::~Entity()
{
}

void Entity::Initialise()
{
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;
	DWORD size = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	void* vertShaderData = nullptr;

	// Load and create the vertex shader.
	HANDLE vsHandle = utils::file::GetFileData( "VertexShader.cso", &vertShaderData, &size );

	hr = device->CreateVertexShader( vertShaderData, size,
		nullptr, &m_vertexShader );
	ASSERT_HANDLE( hr );

	// Create input layout.
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[ 2 ] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};

	hr = device->CreateInputLayout( s_inputElementDesc, _countof( s_inputElementDesc ),
		vertShaderData, size,
		&m_inputLayout );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( vsHandle );

	void* pixelShaderData = nullptr;

	// Handle loading and creating the pixel shader
	HANDLE pxHandle = utils::file::GetFileData( "PixelShader.cso", &pixelShaderData, &size );

	hr = device->CreatePixelShader( pixelShaderData, size,
		nullptr, &m_pixelShader );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( pxHandle );

	CD3D11_BUFFER_DESC worldBufferDesc( sizeof( DirectX::XMMATRIX ), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE );
	device->CreateBuffer( &worldBufferDesc, nullptr, &m_constantBuffer );
}

void Entity::Shutdown()
{
	m_constantBuffer->Release();

	m_inputLayout->Release();
	m_pixelShader->Release();
	m_vertexShader->Release();

	if( m_vertexBuffer != nullptr )
		m_vertexBuffer->Release();
}

void Entity::Update()
{
}

void Entity::Render()
{
	HRESULT hr = S_OK;

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const deviceContext = deviceResources->GetD3DDeviceContext();

	// Get the orientation matrix ready for a world matrix
	XMMATRIX worldMatrix = m_orientation;

	// Set the position row of the world matrix
	worldMatrix.r[ 3 ] = m_position;

	// Matrix needs to be transposed for the vertex shader
	XMMATRIX worldMatrixTransposed = XMMatrixTranspose( worldMatrix );

	ASSERT( m_constantBuffer != nullptr, "Constant buffer doesn't exist. Has View::Initialise() been called?\n" );

	// Map the buffer data
	D3D11_MAPPED_SUBRESOURCE mapped;
	hr = deviceContext->Map( m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped );
	ASSERT_HANDLE( hr );
	memcpy( mapped.pData, &worldMatrixTransposed, sizeof( XMMATRIX ) );
	deviceContext->Unmap( m_constantBuffer, 0 );

	// Actually set the buffer
	deviceContext->VSSetConstantBuffers( 1, 1, &m_constantBuffer );

	// Set input assembler state.
	deviceContext->IASetInputLayout( m_inputLayout );

	// Set shaders.
	deviceContext->VSSetShader( m_vertexShader, nullptr, 0 );
	deviceContext->GSSetShader( nullptr, nullptr, 0 );
	deviceContext->PSSetShader( m_pixelShader, nullptr, 0 );
}

void Entity::SetPosition( const DirectX::XMVECTOR position )
{
	m_position.v = position;
	m_position.f[ 3 ] = 1.0f;
}

void Entity::SetOrientation( const DirectX::XMMATRIX& orientation )
{
	m_orientation = orientation;
}

} //namespace scene