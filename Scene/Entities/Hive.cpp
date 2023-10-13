#include "GameProgExercise01.h"
#include "Core.h"
#include "Scene\Scene.h"
#include "Scene\EntityHelper.h"
#include "Scene\Entities\Hive.h"

using namespace DirectX;

namespace scene
{

Hive::Hive( const UINT hiveIndex ) :
	m_id( hiveIndex ),
	m_numVertices( 0 )
{
	m_shaderType = Scene::ShaderTypes::Lit;
}

Hive::~Hive()
{
}

void Hive::Initialise()
{
	Entity::Initialise();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	const Entity::VertexLit* const cubeVertices = helper::GetCubeVertices( m_numVertices );

	// Create the sides of the box
	VertexLit* const vertices = new VertexLit[m_numVertices];
	ASSERT( vertices != nullptr, "Couldn't create vertices.\n" );
	memcpy( vertices, cubeVertices, sizeof( VertexLit ) * m_numVertices );

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = vertices;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = m_numVertices * sizeof( VertexLit );
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof( VertexLit );

	// Note the vertex buffer is released in the base class
	hr = device->CreateBuffer( &bufferDesc, &initialData,
		&m_vertexBuffer );
	ASSERT_HANDLE( hr );

	delete[] vertices;
}

void Hive::Shutdown()
{
	Entity::Shutdown();
}

void Hive::Update()
{
	Entity::Update();
}

void Hive::Render()
{
	Entity::Render();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const context = deviceResources->GetD3DDeviceContext();

	// Draw triangle.
	UINT strides = sizeof( VertexLit );
	UINT offsets = 0;
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	context->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &strides, &offsets );
	context->Draw( m_numVertices, 0 );
}

} // namespace Hive