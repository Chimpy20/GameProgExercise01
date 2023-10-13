#pragma once

#include "Scene\Entity.h"

namespace scene
{

class Hive : public Entity
{
public:
	Hive( const UINT hiveIndex );
	virtual ~Hive();

	virtual void			Initialise();
	virtual void			Shutdown();

	virtual void			Update();
	virtual void			Render();

private:
	UINT					m_id;
	UINT					m_numVertices;
};

}
