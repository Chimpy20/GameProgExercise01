#pragma once

#include "Scene\Entity.h"

namespace scene
{

class FlyingInsect : public Entity
{
public:
	enum MovementState
	{
		Idle = 0,
		Cruising,
		GoingToTarget,

		Num
	};

	FlyingInsect();
	~FlyingInsect();

	virtual void			Initialise();
	virtual void			Shutdown();

	virtual void			Update();
	virtual void			Render();

	virtual void			Spawn();

	void					RequestMovementState( MovementState state );
	MovementState			GetMovementState() const
	{
		return m_movementState;
	}

	inline void				SetDesiredSpeed( const float desiredSpeed )
	{
		m_desiredSpeed = desiredSpeed;
	}
	inline void				SetDesiredOrientation( const DirectX::XMVECTOR& desiredDirection )
	{
#ifdef _DEBUG
		DirectX::XMVECTOR length = DirectX::XMVector3LengthEst( desiredDirection );
		ASSERT( ( length.m128_f32[ 0 ] < ( 1.0f + Epsilon ) ) && ( length.m128_f32[ 0 ] > ( 1.0f - Epsilon ) ), "Desired Orientation vector isn't normalised.\n" );
#endif
		m_desiredOrienation = desiredDirection;
	}

	inline void				SetTargetPosition( const DirectX::XMVECTOR& targetPosition )
	{
		m_targetPosition = targetPosition;
	}

	virtual void			OnTargetReached() {}

protected:
	static const UINT NumVertices = 6 * 3 * 2;
	static const FlyingInsect::VertexLit InsectBoxVertices[ NumVertices ];
	static const float LerpRate;
	static const float Acceleration;
	static const float SpawnRadius;
	static const float CruiseHeight;
	static const float TargetTriggerDistance;

	inline void				SetColour( const DirectX::XMFLOAT4& colour )
	{
		m_colour = colour;
	}

	float					m_speed;
	UINT					m_padding[ 3 ]{};

private:
	DirectX::XMFLOAT4		m_colour;
	DirectX::XMVECTOR		m_desiredOrienation;
	DirectX::XMVECTOR		m_targetPosition;
	float					m_desiredSpeed;
	MovementState			m_movementState;
};

} // namespace scene

