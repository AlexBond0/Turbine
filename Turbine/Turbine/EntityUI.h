#pragma once

#include "ImGui//imgui.h"
#include "DebugUIContainer.h"

#include "Entity.h"
#include "World.h"

class EntityUI
	: public DebugUIContainer
{
public:
	EntityUI();
	~EntityUI();

	void Render();

	World* world;
	Entity* currentEntity;

private:

	void _RenderEntity();
	void _RenderObj();
	void _RenderInstance();
	void _RenderParticle();
	void _renderPrimitive();
	void _RenderCamera();
	void _RenderLight();
};

