#include "GameProgExercise01.h"
#include "Scene\Scene.h"
#include "Core.h"
#include "Ground.h"

namespace scene
{

const float Ground::HalfWidth = 5.0f;

Ground::Ground()
{
}

Ground::~Ground()
{
}

void Ground::Initialise()
{
	Entity::Initialise();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	// Create vertex buffer.
	static const Vertex VertexData[ NumVertices ] =
	{
		{ { -HalfWidth, 0.0f, HalfWidth, 1.0f },{ 0.05f, 0.9f, 0.1f, 1.0f } },
		{ { HalfWidth,  0.0f, HalfWidth, 1.0f },{ 0.05f, 0.9f, 0.1f, 1.0f } },
		{ { -HalfWidth, 0.0f, -HalfWidth, 1.0f },{ 0.05f, 0.9f, 0.1f, 1.0f } },
		{ { HalfWidth, 0.0f, -HalfWidth, 1.0f },{ 0.05f, 0.9f, 0.1f, 1.0f } }

	};

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = VertexData;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof( VertexData );
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof( Vertex );

	// Note the vertex buffer is released in the base class
	hr = device->CreateBuffer( &bufferDesc, &initialData,
		&m_vertexBuffer );
	ASSERT_HANDLE( hr );

	SetScale( 1.0f );
}

void Ground::Shutdown()
{
	Entity::Shutdown();
}

void Ground::Update()
{
	Entity::Update();
}

void Ground::Render()
{
	Entity::Render();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const context = deviceResources->GetD3DDeviceContext();

	// Draw triangle.
	UINT strides = sizeof( Vertex );
	UINT offsets = 0;
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	context->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &strides, &offsets );
	context->Draw( NumVertices, 0 );
}

} // namespace scene

