#pragma once

#include "Matrix.h"
#include "Color.h"

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

	float* GetDirection();
	float* GetHalfplane();
};

