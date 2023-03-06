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
	m_testObject1( nullptr ),
	m_testObject2( nullptr ),
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
	// Replace this with some proper camera code
	Core* const core = Core::Get();
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