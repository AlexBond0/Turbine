#include "Light.h"



Light::Light(std::string name)
	: Entity(name, EntityType::LIGHT),
	lightUI(this)
{

	lightUI.GetObject3D()->SetScale(2.0);
}


Light::~Light() {

}

 // Return the direction of the light, stored in the up vector
glm::vec3& Light::GetLightDirection() {

	_direction = glm::eulerAngles(_orientation);

	return _direction;
}


// Calculate the half plane for sending to the GLSL shaders
void Light::CalculateHalfPlane(glm::vec3 cameraPosition) {

	halfplane = glm::normalize(cameraPosition + (_up * _orientation));
}

// Create a default sun light
void Light::CreateSun() {

	// direction = glm::vec3(1.0, 1.0, 1.0);
	PointAt(glm::vec3(1.0, 1.0, 1.0));

	ambient = color4(0.4, 0.4, 0.4, 1.0);
	diffuse = color4(0.7, 0.7, 0.7, 1.0);
	specular = color4(1.0, 1.0, 1.0, 1.0);
}

// Create a default moon light
void Light::CreateMoon() {

	// direction = glm::vec3(1.0, 0.5, 0.0);
	PointAt(glm::vec3(1.0, 0.5, 0.0));

	ambient = color4(0.1, 0.1, 0.1, 1.0);
	diffuse = color4(0.5, 0.5, 0.7, 1.0);
	specular = color4(0.3, 0.3, 0.3, 1.0);
}

void Light::OnRender(RenderingContext& rcontext) {

	lightUI.RenderUI(rcontext);
}

Object3D* Light::OnPick() {

	return lightUI.GetObject3D();
}