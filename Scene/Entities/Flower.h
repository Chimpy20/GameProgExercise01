#pragma once

#include "Scene\Entity.h"

namespace scene
{

class Flower : public Entity
{
public:
	static const UINT NumPetals = 8;
	static const float YOffset;
	static const float BaseRadius;
	static const float RimRadius;
	static const float PetalHeight;
	static const float PetalWidth;
	static const DirectX::XMFLOAT4 CentreColourFull;
	static const DirectX::XMFLOAT4 RimColourFull;

	static const float InitialNectarLevelMin;
	static const float InitialNectarLevelMax;
	static const float NectarRechargeRate;

	Flower();
	~Flower();

	void				Initialise();
	void				Shutdown();

	void				Update();
	void				Render();

	inline float		GetNectarLevel() const
	{
		return m_nectarLevel;
	}

	void				Deplete();

private:
	float				m_nectarLevel;
};

} // namespace scene
