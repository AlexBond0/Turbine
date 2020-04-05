#pragma once

#include "Matrix.h"

class Light
{
public:

	float direction[3];
	float halfplane[3];
	float ambient[4];
	float diffuse[4];
	float specular[4];

	Light();
	~Light();

	void CalculateHalfPlane(float* cameraPosition);
	void CreateSun();
	void CreateMoon();
};

