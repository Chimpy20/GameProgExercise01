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

	void						Initialise();
	void						Shutdown();

	void						Update();
	void						Render();

	// Mutators
	void						SetPosition( const DirectX::XMVECTOR position );
	void						SetOrientation( const DirectX::XMMATRIX& orientation );
	void						SetScale( const float scale );

	// Accessors
	inline const DirectX::XMVECTOR&	GetPosition() const
	{
		return m_position.v;
	}

	inline const DirectX::XMVECTORF32& GetPositionAsF32() const
	{
		return m_position;
	}

	inline const DirectX::XMMATRIX&	GetOrientation() const
	{
		return m_orientation;
	}

protected:
	ID3D11InputLayout*			m_inputLayout;
	ID3D11VertexShader*			m_vertexShader;
	ID3D11PixelShader*			m_pixelShader;
	ID3D11Buffer*				m_vertexBuffer;
	ID3D11Buffer*				m_constantBuffer;

	DirectX::XMVECTORF32		m_position;
	DirectX::XMMATRIX			m_orientation;
	float						m_scale;
};

} // namespace scene
