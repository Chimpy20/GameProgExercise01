#pragma once

#include "Scene\Entity.h"

namespace scene
{

class Flower : public Entity
{
public:
	static const UINT NumPetals = 8;
	static const float BaseRadius;
	static const float RimRadius;
	static const float PetalHeight;
	static const float PetalWidth;
	static const DirectX::XMFLOAT4 CentreColourFull;
	static const DirectX::XMFLOAT4 RimColourFull;

	Flower();
	~Flower();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();
private:
};

} // namespace scene
