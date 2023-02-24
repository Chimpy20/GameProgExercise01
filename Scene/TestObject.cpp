#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "TestObject.h"

namespace scene
{

struct Vertex
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
};

TestObject::TestObject():
	m_inputLayout( nullptr ),
	m_vertexBuffer( nullptr ),
	m_vertexShader( nullptr ),
	m_pixelShader( nullptr )
{
}

void TestObject::Initialise()
{
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;
	DWORD size = 0;

	auto device = deviceResources->GetD3DDevice();

	void* vertShaderData = nullptr;

	// Load and create the vertex shader.
	HANDLE vsHandle = utils::file::GetFileData( "VertexShader.cso", &vertShaderData, &size );

	hr = device->CreateVertexShader( vertShaderData, size,
		nullptr, &m_vertexShader );
	ASSERT_HANDLE( hr );

	// Create input layout.
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[ 2 ] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};

	hr = device->CreateInputLayout( s_inputElementDesc, _countof( s_inputElementDesc ),
		vertShaderData, size,
		&m_inputLayout );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( vsHandle );

	void* pixelShaderData = nullptr;

	// Handle loading and creating the pixel shader
	HANDLE pxHandle = utils::file::GetFileData( "PixelShader.cso", &pixelShaderData, &size );

	hr = device->CreatePixelShader( pixelShaderData, size,
		nullptr, &m_pixelShader );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( pxHandle );

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

	hr = device->CreateBuffer( &bufferDesc, &initialData,
		&m_vertexBuffer );
	ASSERT_HANDLE( hr );

}

void TestObject::Shutdown()
{
	m_inputLayout->Release();
	m_vertexBuffer->Release();
	m_pixelShader->Release();
	m_vertexShader->Release();
}

void TestObject::Update()
{
}

void TestObject::Render()
{
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	auto context = deviceResources->GetD3DDeviceContext();

	// Set input assembler state.
	context->IASetInputLayout( m_inputLayout );

	UINT strides = sizeof( Vertex );
	UINT offsets = 0;
	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	context->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &strides, &offsets );

	// Set shaders.
	context->VSSetShader( m_vertexShader, nullptr, 0 );
	context->GSSetShader( nullptr, nullptr, 0 );
	context->PSSetShader( m_pixelShader, nullptr, 0 );

	// Draw triangle.
	context->Draw( NumVertices, 0 );
}

} // namespace scene