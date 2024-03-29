#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Scene.h"
#include "Entity.h"

using namespace DirectX;

namespace scene
{

Entity::Entity() :
	m_vertexBuffer( nullptr ),
	m_constantBuffer( nullptr ),
	m_shaderType( Scene::ShaderTypes::Unlit )
{
	m_colour = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };
	m_orientation = XMMatrixIdentity();
	m_position.v = XMVectorZero();
	m_position.f[ 3 ] = 1.0f;
	m_scale = g_XMOne3;
}

Entity::~Entity()
{
}

void Entity::Initialise()
{
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	CD3D11_BUFFER_DESC shaderConstantDesc( sizeof( ShaderConstants ), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE );
	hr = device->CreateBuffer( &shaderConstantDesc, nullptr, &m_constantBuffer );
	ASSERT_HANDLE( hr );
}

void Entity::Shutdown()
{
	m_constantBuffer->Release();

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
	scene::Scene* const scene = core->GetScene();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const deviceContext = deviceResources->GetD3DDeviceContext();

	ShaderConstants shaderConstants{};

	// Get the orientation matrix ready for a world matrix
	XMMATRIX worldMatrix = m_orientation;

	XMMATRIX scaleMatrix = XMMatrixScaling( m_scale.f[ 0 ], m_scale.f[ 1 ], m_scale.f[ 2 ] );

	XMMATRIX scaledWorldMatrix = XMMatrixMultiply( scaleMatrix, worldMatrix );

	// Set the position row of the world matrix
	scaledWorldMatrix.r[ 3 ] = m_position;

	// Matrix needs to be transposed for the vertex shader
	shaderConstants.worldMatrix = XMMatrixTranspose( scaledWorldMatrix );
	shaderConstants.tint = m_colour;


	ASSERT( m_constantBuffer != nullptr, "Constant buffer doesn't exist. Has View::Initialise() been called?\n" );

	// Map the buffer data
	D3D11_MAPPED_SUBRESOURCE mapped;
	hr = deviceContext->Map( m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped );
	ASSERT_HANDLE( hr );
	memcpy( mapped.pData, &shaderConstants, sizeof( ShaderConstants ) );
	deviceContext->Unmap( m_constantBuffer, 0 );

	// Actually set the buffer
	deviceContext->VSSetConstantBuffers( 1, 1, &m_constantBuffer );

	scene->ActivateShaders( m_shaderType );
}

void Entity::SetPosition( const XMVECTOR position )
{
	m_position.v = position;
	m_position.f[ 3 ] = 1.0f;
}

void Entity::SetOrientation( const XMMATRIX& orientation )
{
#ifdef _DEBUG
	XMVECTOR determinant = XMMatrixDeterminant( orientation );
	ASSERT( ( determinant.m128_f32[ 0 ] < ( 1.0f + Epsilon ) ) && ( determinant.m128_f32[ 0 ] > ( 1.0f - Epsilon ) ), "Orientation matrix isn't normalised.\n" );
#endif
	m_orientation = orientation;
}

// Sets the orientation based on a direction vector assuming up is in the Y axis
void Entity::SetOrientation( const XMVECTOR& orientation )
{
#ifdef _DEBUG
	XMVECTOR length = XMVector3LengthEst( orientation );
	ASSERT( ( length.m128_f32[0] < ( 1.0f + Epsilon ) ) && ( length.m128_f32[ 0 ] > ( 1.0f - Epsilon ) ), "Orientation vector isn't normalised.\n" );
#endif

	XMVECTOR up = XMVECTOR{ 0.0f, 1.0f, 0.0f };
	XMVECTOR lateral = XMVector3Cross( up, orientation );
	lateral = XMVector3Normalize( lateral );
	XMVECTOR relativeUp = XMVector3Cross( orientation, lateral );
	m_orientation.r[ 0 ] = lateral;
	m_orientation.r[ 1 ] = relativeUp;
	m_orientation.r[ 2 ] = orientation;
	m_orientation.r[ 2 ].m128_f32[ 3 ] = 0.0f;
}

void Entity::SetScale( const float scale )
{
	for( UINT index = 0; index < 3; ++index )
		m_scale.f[ index ] = scale;
	m_scale.f[ 3 ] = 0.0f;
}


void Entity::SetScale( const DirectX::XMVECTORF32& scale )
{
	m_scale = scale;
}

const XMVECTOR Entity::GetOrientationAsVector() const
{
	XMVECTOR orientation;
	orientation = m_orientation.r[ 2 ];
	return orientation;
}

} //namespace scene