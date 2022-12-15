#pragma once
#include "System.h"

class EnemyMovementSystem :
    public System
{
public :
    virtual void updateComponents(float delta);
};

