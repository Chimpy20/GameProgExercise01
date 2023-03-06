#include "GameProgExercise01.h"
#include "Core.h"
#include "Scene\Entities\Bee.h"

using namespace DirectX;

namespace scene
{

Bee::Bee()
{
	SetPosition( XMVECTOR{ -3.0f, 3.0f, -3.0f, 0.0f } );
	m_scale.v = XMVECTOR{ 0.25f, 0.15f, 0.2f, 0.0f };
}

Bee::~Bee()
{
}

} // namespace scene