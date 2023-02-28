#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "Core.h"
#include "Utils\File.h"
#include "Scene\Scene.h"
#include "Scene\Entities\TestObject.h"
#include "DX\Input.h"

using namespace DirectX;

namespace scene
{

Scene::Scene() :
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

} // namespace scene