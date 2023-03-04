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
	m_ground->Initialise();
}

void Scene::Shutdown()
{
	if( m_ground != nullptr )
		m_ground->Shutdown();
}

void Scene::Update()
{
	m_camera->Update();
}

void Scene::Render()
{
	m_ground->Render();
}

} // namespace scene