#include "GameProgExercise01.h"
#include "DX\View.h"
#include "DX\Input.h"
#include "Core.h"
#include "Camera.h"

using namespace DirectX;

namespace scene
{

const float  Camera::DefaultDistanceFromFocus = 15.0f;
const float  Camera::DefaultVerticalRotation = XM_2PI * 0.09f;

Camera::Camera() :
	m_horzRotation( 0.0f ),
	m_vertRotation( DefaultVerticalRotation ),
	m_distanceFromLookAt( DefaultDistanceFromFocus )
{
	m_lookAt = XMVectorZero();
}

Camera::~Camera()
{
}

void Camera::Update()
{
	const Core* const core = Core::Get();
	DX::Input* input = core->GetInput();

	const float frameTime = utils::Timers::GetFrameTime();
	float horzRotationThisFrame = input->GetLeftRight() * frameTime;

	m_horzRotation += horzRotationThisFrame;
	if( m_horzRotation < 0.0f )
	{
		m_horzRotation += XM_2PI;
	}
	else if( m_horzRotation > XM_2PI )
	{
		m_horzRotation -= XM_2PI;
	}

	XMVECTOR viewOffset = XMVECTOR{ 0.0f, 0.0f, -DefaultDistanceFromFocus, 1.0f };

	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw( DefaultVerticalRotation, m_horzRotation, 0.0f );

	XMVECTOR transformedOffset = XMVector3Transform( viewOffset, rotationMatrix );

	XMVECTOR viewPos = m_lookAt + transformedOffset;

	// Reverse the offset so we're looking back
	transformedOffset = XMVectorNegate( transformedOffset );

	XMVECTOR viewDirectionNormalised = XMVector3NormalizeEst( transformedOffset );

	DX::View* const view = core->GetView();
	view->SetViewDirection( viewDirectionNormalised );
	view->SetViewPosition( viewPos );

}

}