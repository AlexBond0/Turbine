#include "Light.h"



Light::Light(std::string name)
	: Entity(name, EntityType::LIGHT),
	lightUI(this)
{

	lightUI.GetObject3D()->SetScale(2.0);
	lightUI.GetObject3D()->useLight = false;
}


Light::~Light() {

}

 // Return the direction of the light, stored in the up vector
glm::vec3 Light::GetLightDirection() {

	return glm::eulerAngles(_orientation);
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

	lightUI.GetObject3D()->SetAmbient(diffuse);
	lightUI.RenderUI(rcontext);
}

Object3D* Light::OnPick() {

	return lightUI.GetObject3D();
}

// ==========================================================================

DirectionalLight::DirectionalLight(std::string name)
	: Light(name) {

	lightType = LightType::DIRECTION;
}


// Return the direction of the light
glm::vec3 DirectionalLight::GetLightDirection() {

	return glm::eulerAngles(_orientation);
}

// Return the position of the light
glm::vec3 DirectionalLight::GetLightPosition() {

	return glm::vec3(0);
}

// ==========================================================================

PointLight::PointLight(std::string name) 
	: Light(name) {

	lightType = LightType::POINT;

	constant = 1.0f;
	linear = 0.09;
	quadratic = 0.032;
}

// Return the direction of the light
glm::vec3 PointLight::GetLightDirection() {

	return glm::vec3(0);
}

// Return the position of the light
glm::vec3 PointLight::GetLightPosition() {

	return GetWorldPosition();
}

// ==========================================================================

SpotLight::SpotLight(std::string name)
	: Light(name) {

	lightType = LightType::SPOT;

	constant = 1.0f;
	linear = 0.09;
	quadratic = 0.032;

	cutOff = glm::cos(glm::radians(12.5f));
	outerCutOff = glm::cos(glm::radians(15.0f));
}

// Return the direction of the light
glm::vec3 SpotLight::GetLightDirection() {

	return glm::eulerAngles(_orientation);
}

// Return the position of the light
glm::vec3 SpotLight::GetLightPosition() {

	return GetWorldPosition();
}