#pragma once

#include "Scene\Entity.h"

namespace scene
{

class Ground : public Entity
{
public:
	static const UINT NumVertices = 4;
	static const float HalfWidth;
	Ground();
	~Ground();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

private:
};

}

