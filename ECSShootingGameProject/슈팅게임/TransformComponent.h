#pragma once
#include "Component.h"

class TransformComponent :
    public Component
{
public:
    TransformComponent() {}
    TransformComponent(int X, int Y) : posX(X), posY(Y) {};

protected:
    int posX = 0;
    int posY = 0;

public:
    inline void IncPosX(int x) { posX += x; }
    inline void IncPosY(int y) { posY += y; }
    inline void SetPosX(int x) { posX = x; }
    inline void SetPosY(int y) { posY = y; }
    inline int GetPosX() { return posX; }
    inline int GetPosY() { return posY; }
};

