#pragma once

#include "DebugUIContainer.h"

#include "World.h"

class WorldUI
	: public DebugUIContainer 
{

public:

	WorldUI();
	~WorldUI();

	void Render();

	World* world;
	RenderingContext* rcontext;

private:

	void _RenderEntity(Entity* entity);

	void _RenderEntityRow(Entity* entity);
	void _RenderSelectorButton(Entity* entity);

	int renderer = 0;
};

