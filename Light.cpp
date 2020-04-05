#include "Light.h"



Light::Light()
{
}


Light::~Light()
{
}

void Light::CalculateHalfPlane(float* cameraPosition) {

	float h[3] = {
		cameraPosition[0] + direction[0],
		cameraPosition[1] + direction[1],
		cameraPosition[2] + direction[2],
	};

	MyMatrix::Normalise3(h);

	halfplane[0] = h[0];
	halfplane[1] = h[1];
	halfplane[2] = h[2];
}

void Light::CreateSun() {

	direction[0] = 1.0f;
	direction[1] = 1.0f;
	direction[2] = 1.0f;

	ambient[0] = 0.4f;
	ambient[1] = 0.4f;
	ambient[2] = 0.4f;
	ambient[3] = 1.0f;

	diffuse[0] = 0.7f;
	diffuse[1] = 0.7f;
	diffuse[2] = 0.7f;
	diffuse[3] = 1.0f;

	specular[0] = 1.0f;
	specular[1] = 1.0f;
	specular[2] = 1.0f;
	specular[3] = 1.0f;
}

void Light::CreateMoon() {

	direction[0] = 1.0f;
	direction[1] = 0.5f;
	direction[2] = 0.0f;

	ambient[0] = 0.1f;
	ambient[1] = 0.1f;
	ambient[2] = 0.1f;
	ambient[3] = 1.0f;

	diffuse[0] = 0.5f;
	diffuse[1] = 0.5f;
	diffuse[2] = 0.7f;
	diffuse[3] = 1.0f;

	specular[0] = 0.3f;
	specular[1] = 0.3f;
	specular[2] = 0.3f;
	specular[3] = 1.0f;
}