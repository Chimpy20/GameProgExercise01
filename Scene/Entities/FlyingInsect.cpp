#include "GameProgExercise01.h"
#include "Core.h"
#include "Scene\Entities\FlyingInsect.h"

using namespace DirectX;

namespace scene
{

// Constants
const float FlyingInsect::LerpRate = 1.00f;
const float FlyingInsect::Acceleration = 1.00f;
const float FlyingInsect::CruiseHeight = 2.0f;

FlyingInsect::FlyingInsect() :
	m_speed( 0.0f ),
	m_movementState( MovementState::Idle ),
	m_desiredSpeed( 0.0f )
{
	m_colour = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };

	m_shaderType = Scene::ShaderTypes::Lit;

	m_desiredOrienation = XMVectorZero();
	m_targetPosition = XMVectorZero();

}

FlyingInsect::~FlyingInsect()
{
}

const FlyingInsect::VertexLit FlyingInsect::InsectBoxVertices[ FlyingInsect::NumVertices ] =
{
	// Front face
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

	// Right side
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

	// Back face
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

	// Left side
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

	// Top
	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, 0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, 0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

	// Bottom
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { -0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
	{ { 0.5f, -0.5f, 0.5f, 1.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
};

void FlyingInsect::Initialise()
{
	Entity::Initialise();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	// Create the sides of the box
	VertexLit* const vertices = new VertexLit[ NumVertices ];
	ASSERT( vertices != nullptr, "Couldn't create vertices.\n" );
	memcpy( vertices, InsectBoxVertices, sizeof( VertexLit ) * NumVertices );

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = vertices;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = NumVertices * sizeof( VertexLit );
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof( VertexLit );

	// Note the vertex buffer is released in the base class
	hr = device->CreateBuffer( &bufferDesc, &initialData,
		&m_vertexBuffer );
	ASSERT_HANDLE( hr );

	delete[] vertices;
}

void FlyingInsect::Shutdown()
{
	Entity::Shutdown();
}

void FlyingInsect::Update()
{
	Entity::Update();

	const float frameTime = utils::Timers::GetFrameTime();

	XMVECTOR desiredOrientation = m_desiredOrienation;

	switch( m_movementState )
	{
	case MovementState::Cruising:
		{
			float accelAmount = Acceleration * frameTime;
			if( accelAmount > 1.0f )
				accelAmount = 1.0f;
			m_speed = m_speed + ( ( m_desiredSpeed - m_speed ) * accelAmount );

			// Carry on in current direction at fixed height
			const XMVECTOR delta = XMVectorScale( m_orientation.r[ 2 ], m_speed * frameTime );
			SetPosition( m_position + delta );

			desiredOrientation.m128_f32[ 1 ] += ( ( CruiseHeight - m_position[ 1 ] ) * m_speed * 1.0f );
			desiredOrientation = XMVector3Normalize( desiredOrientation );
		}
		break;

	case MovementState::GoingToTarget:
		m_speed = m_speed + ( ( m_desiredSpeed - m_speed ) * Acceleration * frameTime );
		// Move to target, stop and enter Idle state when reaching it
		break;

	case MovementState::Idle:
	default:
		break;
	}

	float lerpAmount = LerpRate * frameTime;
	if( lerpAmount > 1.0f )
		lerpAmount = 1.0f;
	XMVECTOR newOrientation = XMVectorLerp( m_orientation.r[ 2 ], desiredOrientation, lerpAmount );
	newOrientation = XMVector3Normalize( newOrientation );
	SetOrientation( newOrientation );
}

void FlyingInsect::Render()
{
	Entity::Render();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const context = deviceResources->GetD3DDeviceContext();

	// Draw triangle.
	UINT strides = sizeof( VertexLit );
	UINT offsets = 0;
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	context->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &strides, &offsets );
	context->Draw( NumVertices, 0 );
}

void FlyingInsect::RequestMovementState( MovementState state )
{
	// Handle any one time effects when changing state
	switch( state )
	{
	default:
		break;
	}

	m_movementState = state;
}

} // namespace scene