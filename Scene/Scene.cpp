#include "GameProgExercise01.h"
#include "DX\DeviceResources.h"
#include "DX\Input.h"
#include "Core.h"
#include "Utils\File.h"
#include "Scene\Scene.h"
#include "Scene\Camera.h"
#include "Scene\Entities\Ground.h"
#include "Scene\Entities\Flower.h"
#include "Scene\Entities\Bee.h"
#include "Scene\Entities\Hive.h"

using namespace DirectX;

namespace scene
{

const float Scene::BeeSpawnInterval = 1.0f;
const float Scene::FlowerGridSizeUnits = 10.0f;
const float Scene::FlowerBedSizeUnits = FlowerGridSizeUnits * 1.25f;
const float Scene::NectarThreshold = 0.9f;
const float Scene::HiveDistanceFromOrigin = FlowerGridSizeUnits;


const XMFLOAT4 Scene::HiveColours[ NumHives ] =
{
	XMFLOAT4{ 0.8f, 0.3f, 0.3f, 1.0f }, // red
	XMFLOAT4{ 0.3f, 0.8f, 0.3f, 1.0f }, // green
	XMFLOAT4{ 0.3f, 0.3f, 0.8f, 1.0f }, // blue
	XMFLOAT4{ 0.7f, 0.7f, 0.15f, 1.0f }, // yellow
	XMFLOAT4{ 0.15f, 0.7f, 0.7f, 1.0f },  // cyan
	XMFLOAT4{ 0.7f, 0.15f, 0.7f, 1.0f }  // magenta
};

Scene::Scene() :
	m_camera( nullptr ),
	m_ground( nullptr ),
	m_beeList( nullptr ),
	m_flowerList( nullptr ),
	m_hiveList( nullptr ),
	m_beeSpawnTimer( 0.0f )
{
	m_camera = new Camera();

	m_ground = new Ground();

	m_beeList = new containers::List< Bee* >();
	m_flowerList = new containers::List< Flower* >();
	m_hiveList = new containers::List< Hive* >();
}

Scene::~Scene()
{
	if( m_camera != nullptr )
		delete m_camera;

	// delete each bee
	BeeListItor beeItor = m_beeList->begin();
	while( beeItor != m_beeList->end() )
	{
		Bee* bee = *beeItor;
		delete bee;
		++beeItor;
	}

	if( m_beeList != nullptr )
		delete m_beeList;

	// delete each flower
	FlowerListItor flowerItor = m_flowerList->begin();
	while( flowerItor != m_flowerList->end() )
	{
		Flower* flower = *flowerItor;
		delete flower;
		++flowerItor;
	}

	if( m_flowerList != nullptr )
		delete m_flowerList;

	// delete each hive
	HiveListItor hiveItor = m_hiveList->begin();
	while ( hiveItor != m_hiveList->end() )
	{
		Hive* hive = *hiveItor;
		delete hive;
		++hiveItor;
	}

	if ( m_hiveList != nullptr )
		delete m_hiveList;

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

	m_ground->Initialise();
	m_ground->SetScale( FlowerBedSizeUnits );

	// Create a grid of NxX flowers
	const float FlowerGridSizeAsFloat = (float)( FlowerGridSize - 1 );
	for( UINT gridX = 0; gridX < FlowerGridSize; gridX++ )
	{
		for( UINT gridZ = 0; gridZ < FlowerGridSize; gridZ++ )
		{
			Flower* newFlower = new Flower();
			newFlower->Initialise();
			m_flowerList->push_back( newFlower );
			const float xPos = ( ( ( (float)( gridX ) / FlowerGridSizeAsFloat ) - 0.5f ) * FlowerGridSizeUnits );
			const float zPos = ( ( ( (float)( gridZ ) / FlowerGridSizeAsFloat ) - 0.5f ) * FlowerGridSizeUnits );
			newFlower->SetPosition( XMVECTOR{ xPos, 0.0f, zPos } );
		}
	}

	// Create N hives
	for( UINT hiveIndex = 0; hiveIndex < NumHives; ++hiveIndex )
	{
		Hive* newHive = new Hive( hiveIndex );
		newHive->Initialise();
		m_hiveList->push_back( newHive );
		const float angle = XM_2PI / (float)( NumHives ) * (float)( hiveIndex );
		const float xPos = XMScalarSin( angle ) * HiveDistanceFromOrigin;
		const float zPos = XMScalarCos( angle ) * HiveDistanceFromOrigin;
		newHive->SetPosition( XMVECTOR{ xPos, 1.0f, zPos } );
		newHive->SetScale( XMVECTORF32{ 1.0f, 2.0f, 1.0f } );
		XMVECTOR orientation = XMVECTOR{ -xPos, 0.0f, -zPos, 0.0f };
		orientation = XMVector3Normalize( orientation );
		newHive->SetOrientation( orientation );
		newHive->SetColour( HiveColours[ hiveIndex ] );
	}
}

void Scene::Shutdown()
{
	// Shutdown each bee
	BeeListItor itor = m_beeList->begin();
	while( itor != m_beeList->end() )
	{
		Bee* bee = *itor;
		bee->Shutdown();
		++itor;
	}

	// Shutdown each flower
	FlowerListItor flowerItor = m_flowerList->begin();
	while( flowerItor != m_flowerList->end() )
	{
		Flower* flower = *flowerItor;
		flower->Shutdown();
		++flowerItor;
	}

	// Shutdown each hive	
	HiveListItor hiveItor = m_hiveList->begin();
	while( hiveItor != m_hiveList->end() )
	{
		Hive* hive = *hiveItor;
		hive->Shutdown();
		++hiveItor;
	}

	m_ground->Shutdown();

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
	m_camera->Update();

	const float frameTime = utils::Timers::GetFrameTime();

	// Spawn a bee periodically
	m_beeSpawnTimer -= frameTime;
	if( ( m_beeSpawnTimer <= 0.0f ) && ( m_beeList->size() < MaxBees ) )
	{
		m_beeSpawnTimer += BeeSpawnInterval;

		Bee* const newBee = new Bee();
		newBee->Initialise();
		newBee->Spawn();
		m_beeList->push_back( newBee );
	}

	// Update each bee
	BeeListItor itor = m_beeList->begin();
	while( itor != m_beeList->end() )
	{
		Bee* bee = *itor;
		if( bee->IsMarkedForKill() )
		{
			bee->Shutdown();
			delete bee;
			itor = m_beeList->erase( itor );
			continue;
		}
		else
		{
			bee->Update();
		}
		++itor;
	}

	// Update flowers
	FlowerListItor flowerItor = m_flowerList->begin();
	while( flowerItor != m_flowerList->end() )
	{
		Flower* flower = *flowerItor;
		flower->Update();
		++flowerItor;
	}

	// Update each hive	
	HiveListItor hiveItor = m_hiveList->begin();
	while ( hiveItor != m_hiveList->end() )
	{
		Hive* hive = *hiveItor;
		hive->Update();
		++hiveItor;
	}
}

void Scene::Render()
{
	m_ground->Render();

	// Draw each flower
	FlowerListItor flowerItor = m_flowerList->begin();
	while( flowerItor != m_flowerList->end() )
	{
		Flower* flower = *flowerItor;
		flower->Render();
		++flowerItor;
	}

	// Render each bee
	BeeListItor itor = m_beeList->begin();
	while( itor != m_beeList->end() )
	{
		Bee* bee = *itor;
		bee->Render();
		++itor;
	}

	// Render each hive	
	HiveListItor hiveItor = m_hiveList->begin();
	while ( hiveItor != m_hiveList->end() )
	{
		Hive* hive = *hiveItor;
		hive->Render();
		++hiveItor;
	}
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

Flower* Scene::GetFlowerWithMostNectar()
{
	Flower* mostNectarFlower = nullptr;

	// Make a sub list of flowers with the most nectar
	m_flowerList->sort();

	UINT numSuitableFlowers = 0;

	containers::List< Flower* > flowerSubList;

	FlowerListItor flowerItor = m_flowerList->begin();
	flowerSubList.push_back( *flowerItor );
	++flowerItor;
	++numSuitableFlowers;

	while( flowerItor != m_flowerList->end() )
	{
		Flower* flower = *flowerItor;
		const float nectarLevel = flower->GetNectarLevel();
		if( nectarLevel > NectarThreshold )
		{
			flowerSubList.push_back( flower );
			++numSuitableFlowers;
		}
		++flowerItor;
	}

	const UINT randomFlowerID = utils::Rand() % numSuitableFlowers;

	UINT flowerIndex = 0;
	flowerItor = flowerSubList.begin();
	while( flowerItor != flowerSubList.end() )
	{
		if( flowerIndex == randomFlowerID )
		{
			mostNectarFlower = *flowerItor;
			break;
		}
		++flowerItor;
		++flowerIndex;
	}

	ASSERT( mostNectarFlower != nullptr, "Couldn't find a flower with most nectar.\n" );
	return mostNectarFlower;
}

} // namespace scene