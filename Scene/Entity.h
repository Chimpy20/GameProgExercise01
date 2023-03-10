#pragma once

#include "Scene\Scene.h"

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

	struct VertexLit
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT4 color;
	};

	struct ShaderConstants
	{
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMFLOAT4 tint;
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
	void						SetOrientation( const DirectX::XMVECTOR& orientation );
	void						SetScale( const float scale );

	inline void					SetColour( const DirectX::XMFLOAT4 colour )
	{
		m_colour = colour;
	}

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

	const DirectX::XMVECTOR GetOrientationAsVector() const;


protected:
	ID3D11Buffer*				m_vertexBuffer;
	ID3D11Buffer*				m_constantBuffer;

	DirectX::XMVECTORF32		m_position;
	DirectX::XMMATRIX			m_orientation;
	DirectX::XMFLOAT4			m_colour;
	float						m_scale;

	Scene::ShaderTypes			m_shaderType;
};

} // namespace scene
