#pragma once

#include "Scene\Entity.h"

namespace scene
{

class TestObject : public Entity
{
public:
	TestObject();

	virtual void		Initialise();
	virtual void		Shutdown();

	virtual void		Update();
	virtual void		Render();

private:
	static const UINT NumVertices = 3;

};

} // namespace scene
