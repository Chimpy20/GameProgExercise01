#pragma once

namespace scene
{

// Base class for all entities in the scene - handles setting of world matrix etc..
class Entity abstract
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT4 color;
	};

	Entity();
	~Entity();

	void					Initialise();
	void					Shutdown();

	void					Update();
	void					Render();

	void					SetPosition( const DirectX::XMVECTOR position );
	void					SetOrientation( const DirectX::XMMATRIX orientation );

protected:
	ID3D11InputLayout*		m_inputLayout;
	ID3D11VertexShader*		m_vertexShader;
	ID3D11PixelShader*		m_pixelShader;
	ID3D11Buffer*			m_vertexBuffer;
	ID3D11Buffer*			m_constantBuffer;

	DirectX::XMVECTORF32	m_position;
	DirectX::XMMATRIX		m_orientation;
};

} // namespace scene
