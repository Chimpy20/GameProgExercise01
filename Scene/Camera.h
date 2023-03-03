#pragma once

namespace scene
{

class Camera
{
public:
	static const float DefaultDistanceFromFocus;
	static const float DefaultVerticalRotation;
	Camera();
	~Camera();

	void					Update();

private:
	DirectX::XMVECTOR		m_lookAt;
	float					m_horzRotation;
	float					m_vertRotation;
	float					m_distanceFromLookAt;
};

} // namespace scene