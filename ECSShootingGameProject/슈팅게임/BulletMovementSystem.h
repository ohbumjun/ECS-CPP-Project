#pragma once
#include "System.h"
class BulletMovementSystem :
    public System
{
    virtual void updateComponents(float delta);
};

