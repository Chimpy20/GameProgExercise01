#include "GameProgExercise01.h"
#include "Core.h"
#include "Scene\EntityHelper.h"
#include "Scene\Entities\FlyingInsect.h"

using namespace DirectX;

namespace scene
{

// Constants
const float FlyingInsect::LerpRate = 4.0f;
const float FlyingInsect::Acceleration = 0.50f;
const float FlyingInsect::SpawnRadius = 10.0f;
const float FlyingInsect::CruiseHeight = 2.0f;
const float FlyingInsect::TargetTriggerDistance = 0.2f;

FlyingInsect::FlyingInsect() :
	m_speed( 0.0f ),
	m_killSignal( false ),
	m_numVertices( 0 ),
	m_movementState( MovementState::Idle ),
	m_desiredSpeed( 0.0f )
{
	m_colour = XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f };

	m_shaderType = Scene::ShaderTypes::Lit;

	m_desiredOrienation = XMVectorZero();
	m_desiredOrienation.m128_f32[ 2 ] = 1.0f;
	m_targetPosition = XMVectorZero();

}

FlyingInsect::~FlyingInsect()
{
}



void FlyingInsect::Initialise()
{
	Entity::Initialise();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	const Entity::VertexLit* const cubeVertices = helper::GetCubeVertices( m_numVertices );

	// Create the sides of the box
	VertexLit* const vertices = new VertexLit[ m_numVertices ];
	ASSERT( vertices != nullptr, "Couldn't create vertices.\n" );
	memcpy( vertices, cubeVertices, sizeof( VertexLit ) * m_numVertices );

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = vertices;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = m_numVertices * sizeof( VertexLit );
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

void FlyingInsect::Spawn()
{
	//assuming centre of flower bed is at the origin
	const float theta = (float)(utils::Rand() % 100000) / 100000.0f * XM_2PI;

	XMVECTOR spawnPosition;
	spawnPosition.m128_f32[ 0 ] = XMScalarSin( theta ) * SpawnRadius;
	spawnPosition.m128_f32[ 1 ] = CruiseHeight;
	spawnPosition.m128_f32[ 2 ] = XMScalarCos( theta ) * SpawnRadius;
	spawnPosition.m128_f32[ 3 ] = 0.0f;

	SetPosition( spawnPosition );

	// Just face the origin
	XMVECTOR directionToOrigin = -spawnPosition;
	directionToOrigin = XMVector3Normalize( directionToOrigin );
	SetOrientation( directionToOrigin );
}

void FlyingInsect::Update()
{
	Entity::Update();

	const float frameTime = utils::Timers::GetFrameTime();

	XMVECTOR desiredOrientation = m_desiredOrienation;
	float accelAmount = Acceleration * frameTime;
	if( accelAmount > 1.0f )
		accelAmount = 1.0f;

	switch( m_movementState )
	{
	case MovementState::Cruising:
		{
			m_speed = m_speed + ( ( m_desiredSpeed - m_speed ) * accelAmount );

			// Carry on in current direction at fixed height
			const XMVECTOR delta = XMVectorScale( m_orientation.r[ 2 ], m_speed * frameTime );
			SetPosition( m_position + delta );

			desiredOrientation.m128_f32[ 1 ] += ( ( CruiseHeight - m_position[ 1 ] ) * m_speed * 1.0f );
			desiredOrientation = XMVector3Normalize( desiredOrientation );
		}
		break;

	case MovementState::GoingToTarget:
		{
			m_speed = m_speed + ( ( m_desiredSpeed - m_speed ) * accelAmount );

			const XMVECTOR delta = XMVectorScale( m_orientation.r[ 2 ], m_speed * frameTime );
			SetPosition( m_position + delta );

			// Move to target, stop and enter Idle state when reaching it
			XMVECTOR desiredDirection = m_targetPosition - m_position;

			XMVECTOR distanceToTargetVec = XMVector3Length( desiredDirection );
			const float distanceToTarget = *distanceToTargetVec.m128_f32;
			if( distanceToTarget < TargetTriggerDistance )
			{
				RequestMovementState( MovementState::Idle );
				OnTargetReached();
			}

			desiredOrientation = XMVector3Normalize( desiredDirection );
		}
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
	context->Draw( m_numVertices, 0 );
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