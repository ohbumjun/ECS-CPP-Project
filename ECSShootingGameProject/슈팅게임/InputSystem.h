#pragma once
#include "System.h"

class InputSystem : public System
{
public :
	bool Init();
	virtual void updateComponents(float delta);
	
	void SetPlayerHandle(EntityID ID)
	{
		m_PlayerEntityID = ID;
	}
private :
	EntityID m_PlayerEntityID;
};

