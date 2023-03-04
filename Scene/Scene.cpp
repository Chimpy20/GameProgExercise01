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
	m_camera( nullptr ),
	m_testObject1( nullptr ),
	m_testObject2( nullptr ),
	m_ground( nullptr )
{
	m_camera = new Camera();
	m_testObject1 = new TestObject();
	m_testObject2 = new TestObject();

	m_ground = new Ground();
}

Scene::~Scene()
{
	if( m_camera != nullptr )
		delete m_camera;
	if( m_testObject1 != nullptr )
		delete m_testObject1;
	if( m_testObject2 != nullptr )
		delete m_testObject2;
	if( m_ground != nullptr )
		delete m_ground;
}

void Scene::Initialise()
{
	XMVECTOR position;
	XMMATRIX orientation;
	XMVECTOR oriAsVector;

	orientation = XMMatrixRotationY( 0.05f );

	// 1st test object
	m_testObject1->Initialise();

	position = XMVectorSet( -2.0f, 0.0f, 0.0f, 0.0f );
	oriAsVector = XMVectorSet( 0.0f, 0.0f, -1.0f, 0.0f );
	oriAsVector = XMVector3Transform( oriAsVector, orientation );
	m_testObject1->SetPosition( position );
	m_testObject1->SetOrientation( oriAsVector );

	// 2nd test object
	m_testObject2->Initialise();

	position = XMVectorSet( 2.0f, 0.0f, 0.0f, 0.0f );
	m_testObject2->SetPosition( position );
	m_testObject2->SetOrientation( orientation );

	m_ground->Initialise();
}

void Scene::Shutdown()
{
	if( m_testObject2 != nullptr )
		m_testObject2->Shutdown();
	if( m_testObject1 != nullptr )
		m_testObject1->Shutdown();

	if( m_ground != nullptr )
		m_ground->Shutdown();
}

void Scene::Update()
{
	m_camera->Update();

	m_testObject1->Update();
	m_testObject2->Update();
}

void Scene::Render()
{
	m_testObject1->Render();
	m_testObject2->Render();

	m_ground->Render();
}

} // namespace scene