#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Utils\File.h"
#include "Scene\Scene.h"
#include "Scene\Entities\TestObject.h"
#include "DX\Input.h"
#include "DX\View.h"

using namespace DirectX;

namespace scene
{

Scene::Scene()
{
	m_testObject1 = new TestObject();
	m_testObject2 = new TestObject();
}

Scene::~Scene()
{
	delete m_testObject1;
	delete m_testObject2;
}

void Scene::Initialise()
{
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;
	DWORD size = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

	void* vertShaderData = nullptr;

	HANDLE vsHandle = INVALID_HANDLE_VALUE;
	HANDLE psHandle = INVALID_HANDLE_VALUE;

	// ** UNLIT SHADERS **

	// Load and create the vertex shader for the unlit type of vertices
	vsHandle = utils::file::GetFileData( "VertexShader.cso", &vertShaderData, &size );

	hr = device->CreateVertexShader( vertShaderData, size,
		nullptr, &m_shaderData[ ShaderTypes::Unlit ].vertexShader );
	ASSERT_HANDLE( hr );

	// Create input layout.
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescUnlit[ 2 ] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};

	hr = device->CreateInputLayout( s_inputElementDescUnlit, _countof( s_inputElementDescUnlit ),
		vertShaderData, size,
		&m_shaderData[ ShaderTypes::Unlit ].inputLayout );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( vsHandle );

	void* pixelShaderData = nullptr;

	// Handle loading and creating the pixel shader for the unlit type of vertices
	psHandle = utils::file::GetFileData( "PixelShader.cso", &pixelShaderData, &size );

	hr = device->CreatePixelShader( pixelShaderData, size,
		nullptr, &m_shaderData[ ShaderTypes::Unlit ].pixelShader );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( psHandle );



	// ** LIT SHADERS **

	// Load and create the vertex shader for the unlit type of vertices
	vsHandle = utils::file::GetFileData( "VertexShaderLit.cso", &vertShaderData, &size );

	hr = device->CreateVertexShader( vertShaderData, size,
		nullptr, &m_shaderData[ ShaderTypes::Lit ].vertexShader );
	ASSERT_HANDLE( hr );

	// Create input layout.
	static const D3D11_INPUT_ELEMENT_DESC s_inputElementDescLit[ 3 ] =
	{
		{ "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16,  D3D11_INPUT_PER_VERTEX_DATA,  0 },
		{ "COLOR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA , 0 }
	};

	hr = device->CreateInputLayout( s_inputElementDescLit, _countof( s_inputElementDescLit ),
		vertShaderData, size,
		&m_shaderData[ ShaderTypes::Lit ].inputLayout );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( vsHandle );

	pixelShaderData = nullptr;

	// Handle loading and creating the pixel shader for the unlit type of vertices
	psHandle = utils::file::GetFileData( "PixelShaderLit.cso", &pixelShaderData, &size );

	hr = device->CreatePixelShader( pixelShaderData, size,
		nullptr, &m_shaderData[ ShaderTypes::Lit ].pixelShader );
	ASSERT_HANDLE( hr );

	utils::file::CloseFile( psHandle );



	// TODO: Replace this with some proper camera code
	DX::View* const view = core->GetView();
	view->SetViewPosition( XMVECTOR{ -5.0f, 10.0f, 5.0f, 0.0f } );
	XMVECTOR viewDirection = XMVector3Normalize( XMVECTOR{ 5.0f, -10.0f, -5.0f, 0.0f } );
	view->SetViewDirection( viewDirection );

	XMVECTOR position;
	XMMATRIX orientation;

	// 1st test object
	m_testObject1->Initialise();

	position = XMVectorSet( -2.0f, 0.0f, 0.0f, 1.0f );
	m_testObject1->SetPosition( position );

	// 2nd test object
	m_testObject2->Initialise();

	position = XMVectorSet( 2.0f, 0.0f, 0.0f, 1.0f );
	m_testObject2->SetPosition( position );
	orientation = XMMatrixRotationY( XM_PIDIV2 );
	m_testObject2->SetOrientation( orientation );
}

void Scene::Shutdown()
{
	m_testObject2->Shutdown();
	m_testObject1->Shutdown();

	for( UINT shaderTypeIndex = 0; shaderTypeIndex < ShaderTypes::NumShaderTypes; ++shaderTypeIndex )
	{
		if( m_shaderData[ shaderTypeIndex ].inputLayout != nullptr )
		{
			m_shaderData[ shaderTypeIndex ].inputLayout->Release();
		}
		if( m_shaderData[ shaderTypeIndex ].vertexShader != nullptr )
		{
			m_shaderData[ shaderTypeIndex ].vertexShader->Release();
		}
		if( m_shaderData[ shaderTypeIndex ].pixelShader != nullptr )
		{
			m_shaderData[ shaderTypeIndex ].pixelShader->Release();
		}
	}
}

void Scene::Update()
{
	m_testObject1->Update();
	m_testObject2->Update();

	/*Core* const core = Core::Get();
	DX::Input* input = core->GetInput();
	const float leftRight = input->GetLeftRight();
	io::OutputMessage( "%.2f\n", leftRight );*/

}

void Scene::Render()
{
	m_testObject1->Render();
	m_testObject2->Render();
}

void Scene::ActivateShaders( const ShaderTypes shaderType )
{
	ASSERT( shaderType < ShaderTypes::NumShaderTypes, "Shader type ID is out of range.\n" );
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const deviceContext = deviceResources->GetD3DDeviceContext();

	// Set input assembler state.
	deviceContext->IASetInputLayout( m_shaderData[ shaderType ].inputLayout );

	// Set shaders.
	deviceContext->VSSetShader( m_shaderData[ shaderType ].vertexShader, nullptr, 0 );
	deviceContext->GSSetShader( nullptr, nullptr, 0 );
	deviceContext->PSSetShader( m_shaderData[ shaderType ].pixelShader, nullptr, 0 );
}

} // namespace scene