#pragma once

#include "Scene\Entity.h"

namespace scene
{

class FlyingInsect : public Entity
{
	static const UINT NumVertices = 6 * 3 * 2;
	static const FlyingInsect::Vertex InsectBoxVertices[ NumVertices ];

public:

	FlyingInsect();
	~FlyingInsect();

	void					Initialise();
	void					Shutdown();

	void					Update();
	void					Render();

protected:
	void					SetColour( const DirectX::XMFLOAT4 colour )
	{
		m_colour = colour;
	}

	float					m_speed;

private:
	DirectX::XMFLOAT4		m_colour;
};

} // namespace scene

