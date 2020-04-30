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

	// saves making lots of data types each render
	glm::vec3 _worldPos;
	glm::vec3 _localPos;
	glm::vec3 _translation;
	glm::vec3 _scale;
	glm::vec3 _up;
	glm::vec3 _orientation;
	glm::vec3 _rotation;

	bool _orientationFlip;

	bool _orientationSignX_old;
	bool _orientationSignZ_old;

	bool _orientationSignX_new;
	bool _orientationSignZ_new;
};

