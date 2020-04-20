#pragma once

#include "Entity.h"

// Manages all entites in a scene
class World
{
public:
	World();
	~World();


	void AddEntity(Entity* entity);

	// only use when safe

	// void CreateEntity
};

