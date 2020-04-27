#pragma once

#include "Entity.h"
#include "Object3D.h"
#include "Model3D.h"
#include "Particle.h"
#include "Primitive.h"
#include "Camera.h"

class Camera; //forward decleration

// Manages all entites in a scene
class World
{
public:
	World();
	~World();

	void AddEntity(Entity* entity);
	Entity* GetEntity(std::string name);
	std::map<std::string, Entity*> GetAllEntities();
	std::map<std::string, Entity*> GetAllBaseEntities();
	bool DeleteEntity(std::string name);

	Object3D* CreateObject3D(std::string name);
	Object3D* DuplicateObject3D(std::string copyObjectName, std::string newObjectName);
	Object3D* GetObject3D(std::string name);

	void UnpackModel3D(Model3D* model);

	void Clean();
	void Render(RenderingContext& rcontext);

	bool SetActiveCamera(std::string cameraName);
	Camera* GetActiveCamera();

	void UpdateParticles(double timePassed);

	Entity* currentSelectedEntity; // for object focus in world

private:

	std::map<std::string, Entity*> _entities;	// entity pool managed by the world

	std::map<std::string, Camera*> _cameras;	// camera entites in the scene
	Camera* _currentActiveCamera = nullptr;		// currently active camera

	std::map<std::string, Entity*> _renderBase;		// base render entites

	std::map<std::string, Particle*> _particleSystems;	// particle systems in the scene
};

inline std::map<std::string, Entity*> World::GetAllEntities() {

	return _entities;
}

inline std::map<std::string, Entity*> World::GetAllBaseEntities() {

	return _renderBase;
}