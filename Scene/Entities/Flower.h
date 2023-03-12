#pragma once

#include "Scene\Entity.h"

namespace scene
{

class Flower : public Entity
{
public:
	static const UINT NumPetals = 16;
	static const float YOffset;
	static const float BaseRadius;
	static const float RimRadius;
	static const float PetalHeight;
	static const float PetalWidth;
	static const float PetalAlternateWidthMultiplier;
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

	friend bool operator> ( const Flower& l, const Flower& r )
	{
		return( l.m_nectarLevel < r.m_nectarLevel );
	}

	void				Deplete();

private:
	float				m_nectarLevel;
};

} // namespace scene
