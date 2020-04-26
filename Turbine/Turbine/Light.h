#pragma once

#include "Matrix.h"
#include "entity.h"
#include "Color.h"

// Light element to add basic lighing to the scene
class Light
	: public Entity
{
public:

	Light(std::string name);
	~Light();

	void CalculateHalfPlane(glm::vec3 cameraPosition);
	void CreateSun();
	void CreateMoon();

	void OnRender(RenderingContext& rcontext);
	Object3D* OnPick();

	glm::vec3& GetDirection();
	// glm::vec3* GetHalfplane();

	// glm::vec3 direction;
	glm::vec3 halfplane;

	color4 ambient;
	color4 diffuse;
	color4 specular;
};

