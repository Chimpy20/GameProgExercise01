#pragma once

namespace scene
{

class TestObject;
class Camera;
class Ground;
class Flower;
class Bee;

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

	Flower*				GetFlowerWithMostNectar();

	void				KillBee( Bee* const bee );

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

	Camera*				m_camera;
	Ground*				m_ground;
	Flower*				m_flower;
	Bee*				m_bee;
};

} // namespace scene