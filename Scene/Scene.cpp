#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "DX\Input.h"
#include "Core.h"
#include "Utils\File.h"
#include "Scene\Scene.h"
#include "Scene\Camera.h"
#include "Scene\Entities\TestObject.h"
#include "Scene\Entities\Ground.h"

using namespace DirectX;

namespace scene
{

Scene::Scene() :
	m_inputLayout( nullptr ),
	m_vertexShader( nullptr ),
	m_pixelShader( nullptr ),
	m_camera( nullptr ),
	m_ground( nullptr )
{
	m_camera = new Camera();

	m_ground = new Ground();
}

Scene::~Scene()
{
	if( m_camera != nullptr )
		delete m_camera;

	if( m_ground != nullptr )
		delete m_ground;
}

void Scene::Initialise()
{
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	HRESULT hr = 0;
	DWORD size = 0;

	ID3D11Device* const device = deviceResources->GetD3DDevice();

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

	m_ground->Initialise();
}

void Scene::Shutdown()
{
	m_ground->Shutdown();

	m_inputLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void Scene::Update()
{
	m_camera->Update();
}

void Scene::Render()
{
	m_ground->Render();
}

void Scene::ActivateShaders()
{
	Core* const core = Core::Get();

	const DX::DeviceResources* const deviceResources = core->GetDeviceResources();

	ID3D11DeviceContext* const deviceContext = deviceResources->GetD3DDeviceContext();

	// Set input assembler state.
	deviceContext->IASetInputLayout( m_inputLayout );

	// Set shaders.
	deviceContext->VSSetShader( m_vertexShader, nullptr, 0 );
	deviceContext->GSSetShader( nullptr, nullptr, 0 );
	deviceContext->PSSetShader( m_pixelShader, nullptr, 0 );
}

} // namespace scene