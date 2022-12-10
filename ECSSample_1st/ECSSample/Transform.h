#pragma once

#include <iostream>
#include "Component.h"
#include "Vec2D.h"

class Transform : public Component
{
public :
	Transform() = default;

	Transform(float x, float y) {

		std::cout << "input x, y : " << x << "," << y << std::endl;
		position.x = x;
		position.y = y;

		scale.x = 1;
		scale.y = 1;

		rotation = 0.0f;
	}

	Transform(float x, float y, float scx, float scy, float rot) {
		position.x = x;
		position.y = y;

		scale.x = scx;
		scale.y = scy;

		rotation = rot;
	}
	
	virtual ~Transform() = default;

	Vector2 position = Vector2(0.f, 0.f);
	Vector2 scale = Vector2(1.f, 1.f);
	float rotation = 0.f;
};