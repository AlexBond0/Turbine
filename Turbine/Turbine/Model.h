#pragma once

#include "EntityManager.h"

class Model
	: public Entity
	, public EntityManager
{
public:
	Model(std::string name);
	~Model() {};

	Object3D* OnPick();
	void OnRender(RenderingContext& rcontext);

	void Clean();

};

