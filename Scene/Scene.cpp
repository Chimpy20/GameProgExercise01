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

Scene::Scene() :
	m_inputLayout( nullptr ),
	m_vertexShader( nullptr ),
	m_pixelShader( nullptr ),
	m_testObject1( nullptr ),
	m_testObject2( nullptr )
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

	// Replace this with some proper camera code
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

	m_inputLayout->Release();
	m_pixelShader->Release();
	m_vertexShader->Release();
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