#pragma once

#include "FlyingInsect.h"

namespace scene
{

class Flower;

class Bee : public FlyingInsect
{
	enum BeeState
	{
		None = 0,
		PickingFlower,
		GoingToFlower,
		Leaving
	};

	static const float MaxSpeed;
	static const float FlowerActivationRange;

public:
	Bee();
	~Bee();

	virtual void			Initialise();
	virtual void			Update();

	virtual void			OnTargetReached();

private:
	void					PickFlower();

	BeeState				m_beeState;
	Flower*					m_targetFlower;
};

} // namespace scene

