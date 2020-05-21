#pragma once

#include "DebugUIContainer.h"
#include "World.h"

#include <fstream>

#include "LuaEditorUI.h"

class WorldUI
	: public DebugUIContainer 
{

public:

	WorldUI();
	~WorldUI();

	void Render();

	World* world;
	RenderingContext* rcontext;

	LuaEditorUI luaEditor;

	double timePassed = 1.0f;

private:

	void _RenderOutline();
	void _RenderEnviro();
	void _RenderTextures();

	void _RenderEntity(Entity* entity);

	void _ColourActive(Entity* entity);
	void _ColourFinish();

	void _ShowSelectedEntity(Entity* entity);

	void _RenderEntityRow(Entity* entity);
	void _RenderSelectorButton(Entity* entity);

	void _RenderFPS();

	int renderer = 1;

	float _currentfps;
	std::vector<float> _frames;

	const float range = 0.1f;
};

