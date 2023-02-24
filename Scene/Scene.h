#pragma once

namespace scene
{

class TestObject;

class Scene
{
public:
	Scene();
	~Scene();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

private:
	TestObject*			m_testObject;
};

} // namespace scene