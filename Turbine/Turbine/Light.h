#pragma once

#include "Matrix.h"
#include "Color.h"

// Light element to add basic lighing to the scene
class Light
{
public:

	glm::vec3 direction;
	glm::vec3 halfplane;

	color4 ambient;
	color4 diffuse;
	color4 specular;

	Light();
	~Light();

	void CalculateHalfPlane(glm::vec3 cameraPosition);
	void CreateSun();
	void CreateMoon();

	glm::vec3* GetDirection();
	glm::vec3* GetHalfplane();
};

