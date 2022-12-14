#pragma once

class Entity;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;

	Entity* entity = nullptr;

	virtual bool init() { return true; }
	virtual void draw() {};
	virtual void update() {};
};