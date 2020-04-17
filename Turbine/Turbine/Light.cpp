#include "Light.h"



Light::Light()
{
}


Light::~Light()
{
}

// Return handle-friendly direction information
// might be replaced with a glm::value_ptr()
glm::vec3* Light::GetDirection() {

	return &direction;
}

// Return handle-friendly halfplane information
// might be replaced with a glm::value_ptr()
glm::vec3* Light::GetHalfplane() {

	return &halfplane;
}

// Calculate the half plane for sending to the GLSL shaders
void Light::CalculateHalfPlane(glm::vec3 cameraPosition) {

	halfplane = glm::normalize(cameraPosition + direction);
}

// Create a default sun light
void Light::CreateSun() {

	direction = glm::vec3(1.0, 1.0, 1.0);

	ambient = color4(0.4, 0.4, 0.4, 1.0);
	diffuse = color4(0.7, 0.7, 0.7, 1.0);
	specular = color4(1.0, 1.0, 1.0, 1.0);
}

// Create a default moon light
void Light::CreateMoon() {

	direction = glm::vec3(1.0, 0.5, 0.0);

	ambient = color4(0.1, 0.1, 0.1, 1.0);
	diffuse = color4(0.5, 0.5, 0.7, 1.0);
	specular = color4(0.3, 0.3, 0.3, 1.0);
}