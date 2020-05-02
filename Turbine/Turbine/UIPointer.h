#pragma once

#include "Primitive.h"

class UIPointer
{
public:
	UIPointer(Entity* entityToMirror);
	~UIPointer();

	bool showUIObject = false;

	void RenderUI(RenderingContext& rcontext);

	Object3D* GetObject3D();

	void UsePosition(glm::vec3* position);
	void UseTarget(glm::vec3* target);

	void SetColor(color4 color);

	bool showDirection = true;

private:

	bool _usePosition = false;
	glm::vec3* _position;

	bool _useTarget = false;
	glm::vec3* _target;

	std::map<std::string, Primitive*> _uiObjs;	// map of primitives

	Entity* _entityToMirror; // entity to mirror

	void _GenXYZBall(Entity* entityToMirror);
};