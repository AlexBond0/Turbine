#pragma once

#include "EntityManager.h"
#include "Object3D.h"
#include "Primitive.h"
#include "Camera.h"
#include "LightManager.h"
#include "Model.h"
#include "Environment.h"

class Camera; //forward decleration

// Manages all entites in a scene
class World
	: public EntityManager
{
public:
	World() {};
	~World() {};

	// std::map<std::string, Entity*> GetAllBaseEntities();

	bool SetActiveCamera(std::string cameraName);
	Camera* GetActiveCamera();
	std::map<std::string, Camera*> GetAllCameras();

	void AddEntity(Entity* entity);
	bool DeleteEntity(std::string name);

	Entity* GetModelEntity(std::string model, std::string name);
	Object3D* GetModelObject3D(std::string model, std::string name);

	Entity* currentSelectedEntity = nullptr;	// for object focus in world
	LightManager lights;						// light manager for all lights in world

	Environment enviro;

private:

	std::map<std::string, Camera*> _cameras;	// camera entites in the scene
	Camera* _currentActiveCamera = nullptr;		// currently active camera

	// std::map<std::string, Entity*> _renderBase;		// base render entites
	std::map<std::string, Model*> _models;		// base render entites
};


inline std::map<std::string, Camera*> World::GetAllCameras() {

	return _cameras;
}