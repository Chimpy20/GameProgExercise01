#pragma once

#include "Scene\Entity.h"

namespace scene
{

class TestObject : public Entity
{
public:
	TestObject();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

private:
	static const UINT NumVertices = 3;

};

} // namespace scene
