#pragma once

#include "Scene\Entity.h"

namespace scene
{

class Ground : public Entity
{
public:
	static const UINT NumVertices = 4;
	static const float HalfWidth;
	static const DirectX::XMFLOAT4 Colour;

	Ground();
	~Ground();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

private:
};

}

