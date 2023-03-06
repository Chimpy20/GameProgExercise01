#include "GameProgExercise01.h"
#include "Core.h"
#include "Scene\Scene.h"
#include "Scene\Entities\Flower.h"

using namespace DirectX;

namespace scene
{

const float Flower::BaseRadius = 0.1f;
const float Flower::RimRadius = 0.2f;
const float Flower::PetalHeight = 0.1f;
const float Flower::PetalWidth = 0.5f;

const DirectX::XMFLOAT4 Flower::CentreColourFull = XMFLOAT4{ 0.9f, 0.1f, 0.9f, 1.0f };
const DirectX::XMFLOAT4 Flower::RimColourFull = XMFLOAT4{ 1.0f, 1.0f, 0.1f, 1.0f };

Flower::Flower()
{
}

Flower::~Flower()
{
}

void Flower::Initialise()
{
	Entity::Initialise();

	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	// Create the flower petals
	static const UINT NumPetalVertices = NumPetals * 6;
	Vertex* const petalVertices = new Vertex[ NumPetalVertices ];
	ASSERT( petalVertices != nullptr, "Couldn't create petal vertices.\n" );

	const float angleStride = DirectX::XM_2PI / static_cast<float>( NumPetals );
	Vertex* vertex = petalVertices;
	float startingAngle = 0.0f;
	for( UINT petalIndex = 0; petalIndex < NumPetals; ++petalIndex )
	{
		vertex->position.x = DirectX::XMScalarSin( startingAngle ) * BaseRadius;
		vertex->position.y = 0.0f;
		vertex->position.z = DirectX::XMScalarCos( startingAngle ) * BaseRadius;
		vertex->color = CentreColourFull;
		vertex++;

		vertex->position.x = DirectX::XMScalarSin( startingAngle ) * ( BaseRadius + PetalWidth );
		vertex->position.y = PetalHeight;
		vertex->position.z = DirectX::XMScalarCos( startingAngle ) * ( BaseRadius + PetalWidth );
		vertex->color = RimColourFull;
		vertex++;

		vertex->position.x = DirectX::XMScalarSin( startingAngle + angleStride ) * ( BaseRadius + PetalWidth );
		vertex->position.y = PetalHeight;
		vertex->position.z = DirectX::XMScalarCos( startingAngle + angleStride ) * ( BaseRadius + PetalWidth );
		vertex->color = RimColourFull;
		vertex++;

		vertex->position.x = DirectX::XMScalarSin( startingAngle ) * BaseRadius;
		vertex->position.y = 0.0f;
		vertex->position.z = DirectX::XMScalarCos( startingAngle ) * BaseRadius;
		vertex->color = CentreColourFull;
		vertex++;

		vertex->position.x = DirectX::XMScalarSin( startingAngle + angleStride ) * ( BaseRadius + PetalWidth );
		vertex->position.y = PetalHeight;
		vertex->position.z = DirectX::XMScalarCos( startingAngle + angleStride ) * ( BaseRadius + PetalWidth );
		vertex->color = RimColourFull;
		vertex++;

		vertex->position.x = DirectX::XMScalarSin( startingAngle + angleStride ) * BaseRadius;
		vertex->position.y = 0.0f;
		vertex->position.z = DirectX::XMScalarCos( startingAngle + angleStride ) * BaseRadius;
		vertex->color = CentreColourFull;
		vertex++;

		startingAngle += angleStride;
	}

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = petalVertices;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = NumPetalVertices * sizeof( Vertex );
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof( Vertex );

	// Note the vertex buffer is released in the base class
	hr = device->CreateBuffer( &bufferDesc, &initialData,
		&m_vertexBuffer );
	ASSERT_HANDLE( hr );

	if( petalVertices != nullptr )
		delete petalVertices;

	SetScale( 1.0f );
}

void Flower::Shutdown()
{
	Entity::Shutdown();
}

void Flower::Update()
{
	Entity::Update();
}

void Flower::Render()
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
	static const UINT NumPetalVertices = NumPetals * 6;
	context->Draw( NumPetalVertices, 0 );
}

} // namespace scene