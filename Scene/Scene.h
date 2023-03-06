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

	void				ActivateShaders();

private:
	ID3D11InputLayout* m_inputLayout;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;

	TestObject*			m_testObject1;
	TestObject*			m_testObject2;
};

} // namespace scene