#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Utils\File.h"
#include "Scene\Scene.h"
#include "Scene\TestObject.h"

namespace scene
{

Scene::Scene() :
	m_testObject( nullptr )
{
	m_testObject = new TestObject();
}

Scene::~Scene()
{
	delete m_testObject;
}

void Scene::Initialise()
{
	m_testObject->Initialise();
}

void Scene::Shutdown()
{
	m_testObject->Shutdown();
}

void Scene::Update()
{
	m_testObject->Update();
}

void Scene::Render()
{
	m_testObject->Render();
}

} // namespace scene