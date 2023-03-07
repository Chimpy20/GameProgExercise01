#pragma once

namespace scene
{

class TestObject;

// Object containing all the entities and related objects in the scene
class Scene sealed
{
public:
	enum ShaderTypes
	{
		Unlit = 0,
		Lit,

		NumShaderTypes
	};

	Scene();
	~Scene();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

	void				ActivateShaders( const ShaderTypes shaderType );

private:
	struct ShaderData
	{
		ShaderData(): inputLayout( nullptr ),
			vertexShader( nullptr ),
			pixelShader( nullptr )
		{
		}

		ID3D11InputLayout* inputLayout;
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
	};
	
	ShaderData m_shaderData[ ShaderTypes::NumShaderTypes ];

	TestObject*			m_testObject1;
	TestObject*			m_testObject2;
};

} // namespace scene