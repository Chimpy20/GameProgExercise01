#pragma once

namespace scene
{

class TestObject;

// Object containing all the entities and related objects in the scene
class Scene sealed
{
public:
	Scene();
	~Scene();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

private:
	TestObject*			m_testObject1;
	TestObject*			m_testObject2;
};

} // namespace scene