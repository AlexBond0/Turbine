#pragma once

#include "Matrix.h"
#include "entity.h"
#include "Color.h"
#include "Primitive.h"
#include "UIPointer.h"

enum class LightType {
	DIRECTION,
	POINT,
	SPOT
};

#define MAX_LIGHTS = 4;

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

	virtual glm::vec3 GetLightDirection() = 0;
	virtual glm::vec3 GetLightPosition() = 0;

	virtual json Serialize();
	Light(json& data); // Deserialize

	LightType lightType;

	// glm::vec3* GetHalfplane();

	glm::vec3 halfplane;

	color4 ambient;
	color4 diffuse;
	color4 specular;
	float specularStrength;

	bool isActive = true;

	UIPointer lightUI;
};

class DirectionalLight
	: public Light {
public:

	DirectionalLight(std::string name);
	~DirectionalLight() {};

	glm::vec3 GetLightDirection();
	glm::vec3 GetLightPosition();

	virtual json Serialize();
	DirectionalLight(json& data); // Deserialize
};

class PointLight
	: public Light {
public:

	PointLight(std::string name);
	~PointLight() {};

	float constant;
	float linear;
	float quadratic;

	glm::vec3 GetLightDirection();
	glm::vec3 GetLightPosition();

	virtual json Serialize();
	PointLight(json& data); // Deserialize
};

class SpotLight
	: public Light {
public:

	SpotLight(std::string name);
	~SpotLight() {};

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

	float CalculateInsideCutoff();
	float CalculateOutsideCutoff();

	glm::vec3 GetLightDirection();
	glm::vec3 GetLightPosition();

	virtual json Serialize();
	SpotLight(json& data); // Deserialize
};