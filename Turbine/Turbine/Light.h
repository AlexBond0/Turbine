#pragma once

#include "Matrix.h"
#include "entity.h"
#include "Color.h"
#include "Primitive.h"
#include "UIPointer.h"


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

	glm::vec3& GetLightDirection();
	// glm::vec3* GetHalfplane();

	glm::vec3 halfplane;

	color4 ambient;
	color4 diffuse;
	color4 specular;

	UIPointer lightUI;

protected:

	glm::vec3 _direction;

};