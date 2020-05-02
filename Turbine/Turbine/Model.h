#pragma once

#include "Entity.h"


class Model
{
public:
	Model();
	~Model();

private:

	std::map<std::string, Entity*> _entities;	// entity pool managed by the world
};

