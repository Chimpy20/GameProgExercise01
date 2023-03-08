#pragma once

#include "Scene\Entity.h"

namespace scene
{

class FlyingInsect : public Entity
{
	static const UINT NumVertices = 6 * 3 * 2;
	static const FlyingInsect::VertexLit InsectBoxVertices[ NumVertices ];

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

	void					Initialise();
	void					Shutdown();

	void					Update();
	void					Render();

	void					RequestMovementState( MovementState state );

	inline void				SetDesiredSpeed( const float desiredSpeed )
	{
		m_desiredSpeed = desiredSpeed;
	}
	inline void				SetDesiredOrientation( const DirectX::XMVECTOR desiredDirection )
	{
		m_desiredOrienation = desiredDirection;
	}

	inline void				SetTargetPosition( const DirectX::XMVECTOR targetPosition )
	{
		m_targetPosition = targetPosition;
	}

protected:
	inline void				SetColour( const DirectX::XMFLOAT4 colour )
	{
		m_colour = colour;
	}

	float					m_speed;
	UINT					m_padding[ 3 ];

private:
	DirectX::XMFLOAT4		m_colour;
	DirectX::XMVECTOR		m_desiredOrienation;
	DirectX::XMVECTOR		m_targetPosition;
	float					m_desiredSpeed;
	MovementState			m_movementState;
};

} // namespace scene

