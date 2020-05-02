#pragma once

#include "EntityManager.h"


class Model
	: public EntityManager
	, public Entity
{
public:
	Model(std::string name);
	~Model() {};

	Entity* GetBaseEntity();
	bool SetBaseEntity(std::string name);

	void Render(RenderingContext& rcontext);

private:

	Entity* _baseEntity;
};

