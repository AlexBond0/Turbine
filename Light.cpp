#include "Light.h"



Light::Light()
{
}


Light::~Light()
{
}

float* Light::GetDirection() {

	return &direction.x;
}

float* Light::GetHalfplane() {

	return &halfplane.x;
}

void Light::CalculateHalfPlane(glm::vec3 cameraPosition) {

	halfplane = glm::normalize(cameraPosition + direction);
}

void Light::CreateSun() {

	direction = glm::vec3(1.0, 1.0, 1.0);

	ambient = color4(0.4, 0.4, 0.4, 1.0);
	diffuse = color4(0.7, 0.7, 0.7, 1.0);
	specular = color4(1.0, 1.0, 1.0, 1.0);
}

void Light::CreateMoon() {

	direction = glm::vec3(1.0, 0.5, 0.0);

	ambient = color4(0.1, 0.1, 0.1, 1.0);
	diffuse = color4(0.5, 0.5, 0.7, 1.0);
	specular = color4(0.3, 0.3, 0.3, 1.0);
}