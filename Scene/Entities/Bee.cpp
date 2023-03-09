#include "GameProgExercise01.h"
#include "Core.h"
#include "Scene\Entities\Flower.h"
#include "Scene\Entities\Bee.h"

using namespace DirectX;

namespace scene
{

const float Bee::MaxSpeed = 1.5f;
const float Bee::FlowerActivationRange = 1.0f;

Bee::Bee() :
	m_beeState( BeeState::None ),
	m_targetFlower( nullptr )
{
	m_scale.v = XMVECTOR{ 0.25f, 0.15f, 0.2f, 0.0f };
}

Bee::~Bee()
{
}

void Bee::Initialise()
{
	FlyingInsect::Initialise();

	m_beeState = BeeState::PickingFlower;
}

void Bee::Update()
{
	FlyingInsect::Update();

	switch( m_beeState )
	{
	case BeeState::PickingFlower:
		if( m_targetFlower == nullptr )
		{
			PickFlower();
			if( m_targetFlower != nullptr )
			{
				m_beeState = BeeState::GoingToFlower;

				// Get the direction the target flower
				XMVECTOR targetPos = m_targetFlower->GetPosition();
				XMVECTOR desiredDirection = targetPos - m_position;
				desiredDirection = XMVector3Normalize( desiredDirection );

				SetDesiredOrientation( desiredDirection );
				SetDesiredSpeed( MaxSpeed );
				RequestMovementState( MovementState::Cruising );
			}
		}
		break;
	case BeeState::GoingToFlower:
		{
			// Get the direction the target flower
			XMVECTOR targetPos = m_targetFlower->GetPosition();
			XMVECTOR desiredDirection = targetPos - m_position;
			if( GetMovementState() == MovementState::Cruising )
			{
				XMVECTOR desiredDirectionNoY = desiredDirection;
				desiredDirectionNoY.m128_f32[ 1 ] = 0.0f;

				XMVECTOR distanceNoY = XMVector3Length( desiredDirectionNoY );
				if( *distanceNoY.m128_f32 < FlowerActivationRange )
				{
					// Start to go to target
					SetTargetPosition( targetPos );
					SetDesiredSpeed( MaxSpeed * 0.5f );
					RequestMovementState( MovementState::GoingToTarget );
				}
			}
			else
			{
			}
		}
		break;
	case BeeState::Leaving:
		{
			// Get the horizontal distance from the exit perimeter, assuming centre of flower bed is at the origin
			XMVECTOR directionToOrigin = -m_position;
			directionToOrigin.m128_f32[ 1 ] = 0.0f;
			XMVECTOR distanceToOrigin = XMVector3Length( directionToOrigin );
			if( *distanceToOrigin.m128_f32 > SpawnRadius )
			{
				//Core* const core = Core::Get();
				//scene::Scene* const scene = core->GetScene();

				m_killSignal = true;
			}
		}
		break;
	case BeeState::None:
	default:
		break;
	}
}

void Bee::PickFlower()
{
	Core* const core = Core::Get();
	scene::Scene* const scene = core->GetScene();
	m_targetFlower = scene->GetFlowerWithMostNectar();
}

void Bee::OnTargetReached()
{
	ASSERT( m_beeState == BeeState::GoingToFlower, "Should be going to flower.\n" );

	// Reflect orientation in the Y-axis
	XMVECTOR newOrientation = GetOrientationAsVector();
	newOrientation.m128_f32[ 1 ] = -newOrientation.m128_f32[ 1 ];
	SetOrientation( newOrientation );

	RequestMovementState( MovementState::Cruising );
	m_beeState = BeeState::Leaving;
}

} // namespace scene