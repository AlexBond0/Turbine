#pragma once

#include "EntityManager.h"
#include "Object3D.h"
#include "Primitive.h"
#include "Camera.h"
#include "LightManager.h"
#include "Model.h"
#include "ModelLoader.h"
#include "Environment.h"
#include "Texture.h"

class Camera; //forward decleration

// Manages all entites in a scene
class World
	: public EntityManager
{
public:
	World() {};
	~World() {};

	Entity* currentSelectedEntity = nullptr;	// for object focus in world

	LightManager lights;						// light manager for all lights in world
	Environment enviro;							// the world enviroment
	TextureManager textures;					// the set of loaded textures

	bool SetActiveCamera(std::string cameraName);
	Camera* GetActiveCamera();
	std::map<std::string, Camera*> GetAllCameras();

	void AddEntity(Entity* entity);
	bool DeleteEntity(std::string name);

	Entity* GetModelEntity(std::string model, std::string name);
	Object3D* GetModelObject3D(std::string model, std::string name);

	std::string Serialize();
	static World* Deserialize(json& data);

private:

	std::map<std::string, Camera*> _cameras;	// camera entites in the scene
	Camera* _currentActiveCamera = nullptr;		// currently active camera

	std::map<std::string, Model*> _models;		// base render entites

	void _CalculateRelations(json& data, std::map<std::string, Entity*> entites);
	bool _HasEntityBeenLoaded(Entity* entity);
	std::string _GetModelName(Entity* entity);
};


inline std::map<std::string, Camera*> World::GetAllCameras() {

	return _cameras;
}