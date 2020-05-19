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

	lightUI.SetColor(diffuse);
	lightUI.RenderUI(rcontext);
}

Object3D* Light::OnPick() {

	return lightUI.GetObject3D();
}

json Light::Serialize() {

	json me = Entity::Serialize();

	me["ambient"] = ambient.rgba;
	me["diffuse"] = diffuse.rgba;
	me["specular"] = specular.rgba;
	me["specularStrength"] = specularStrength;

	me["isActive"] = isActive;

	// pack and send json
	json ret;
	ret["Light"] = me;
	return ret;
}

Light::Light(json& data)
	: Entity(data["entity"])
	, lightUI(this) {

	ambient		= color4(data["ambient"]);
	diffuse		= color4(data["diffuse"]);
	specular	= color4(data["specular"]);
	specularStrength = data["specularStrength"];

	isActive = data["isActive"];

	lightUI.GetObject3D()->SetScale(2.0);
	lightUI.GetObject3D()->useLight = false;
}

// ==========================================================================

DirectionalLight::DirectionalLight(std::string name)
	: Light(name) {

	lightType = LightType::DIRECTION;

	useTarget = true;

	lightUI.UsePosition(&_localPos);
	lightUI.UseTarget(&_target);
}


// Return the direction of the light
glm::vec3 DirectionalLight::GetLightDirection() {

	return (_target - _localPos);
}

// Return the position of the light
glm::vec3 DirectionalLight::GetLightPosition() {

	return glm::vec3(0);
}

json DirectionalLight::Serialize() {

	json ret;
	ret["DirectionalLight"] = Light::Serialize();
	return ret;
}

DirectionalLight::DirectionalLight(json& data) 
	: Light(data["Light"]) {

	lightType = LightType::DIRECTION;
	useTarget = true;

	lightUI.UsePosition(&_localPos);
	lightUI.UseTarget(&_target);
}

// ==========================================================================

PointLight::PointLight(std::string name) 
	: Light(name) {

	lightType = LightType::POINT;
	lightUI.showDirection = false;

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

json PointLight::Serialize() {

	json me = Light::Serialize();

	me["constant"] = constant;
	me["linear"] = linear;
	me["quadratic"] = quadratic;

	// pack and send json
	json ret;
	ret["PointLight"] = me;
	return ret;
}

PointLight::PointLight(json& data)
	: Light(data["Light"]) {

	lightType = LightType::POINT;
	lightUI.showDirection = false;

	constant	= data["constant"];
	linear		= data["linear"];
	quadratic	= data["quadratic"];
}

// ==========================================================================

SpotLight::SpotLight(std::string name)
	: Light(name) {

	lightType = LightType::SPOT;

	useTarget = true;

	lightUI.UsePosition(&_localPos);
	lightUI.UseTarget(&_target);

	constant = 1.0f;
	linear = 0.09;
	quadratic = 0.032;

	cutOff = 12.5f;
	outerCutOff = 15.0f;
}

float SpotLight::CalculateInsideCutoff() {

	return glm::cos(glm::radians(cutOff));
}

float SpotLight::CalculateOutsideCutoff() {

	return glm::cos(glm::radians(outerCutOff));
}

// Return the direction of the light
glm::vec3 SpotLight::GetLightDirection() {

	return (_target - _localPos);
}

// Return the position of the light
glm::vec3 SpotLight::GetLightPosition() {

	return GetWorldPosition();
}

json SpotLight::Serialize() {

	json me = Light::Serialize();

	me["constant"] = constant;
	me["linear"] = linear;
	me["quadratic"] = quadratic;

	me["cutOff"] = cutOff;
	me["outerCutOff"] = outerCutOff;

	// pack and send json
	json ret;
	ret["SpotLight"] = me;
	return ret;
}

SpotLight::SpotLight(json& data)
	: Light(data["Light"]) {

	lightType = LightType::SPOT;

	constant	= data["constant"];
	linear		= data["linear"];
	quadratic	= data["quadratic"];

	cutOff		= data["cutOff"];
	outerCutOff	= data["outerCutOff"];
}