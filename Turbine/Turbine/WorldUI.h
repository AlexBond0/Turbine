#pragma once

#include "ImGui//imgui.h"

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

private:

	void _RenderEntity(Entity* entity);

	void _RenderEntityRow(Entity* entity);

	ImVec4 objCol		= ImVec4(0.0f, 0.7941f, 1.0f, 1.0f);
	ImVec4 insanceCol	= ImVec4(0.42f, 0.67f, 0.70f, 1.0f);
	ImVec4 particleCol	= ImVec4(0.01f, 0.89f, 0.58f, 1.0f);
	ImVec4 primitiveCol = ImVec4(0.53f, 0.76f, 0.0f, 1.0f);
	ImVec4 cameraCol	= ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
};

