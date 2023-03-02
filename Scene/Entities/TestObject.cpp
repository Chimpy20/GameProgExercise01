#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "TestObject.h"

namespace scene
{

TestObject::TestObject()
{
}

void TestObject::Initialise()
{
	Entity::Initialise();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	// Create vertex buffer.
	static const Vertex VertexData[ NumVertices ] =
	{
		{ { 0.0f, 0.0f,  0.5f, 1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },  // Top
		{ { 0.5f,  0.0f,  0.0f, 1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },  // Right
		{ { -0.5f, 0.0f,  0.0f, 1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },  // Left

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

void TestObject::Shutdown()
{
	Entity::Shutdown();
}

void TestObject::Update()
{
	Entity::Update();
}

void TestObject::Render()
{
	Entity::Render();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const context = deviceResources->GetD3DDeviceContext();

	// Draw triangle.
	UINT strides = sizeof( Vertex );
	UINT offsets = 0;
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	context->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &strides, &offsets );
	context->Draw( NumVertices, 0 );
}

} // namespace scene