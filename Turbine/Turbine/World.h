#pragma once

#include "Entity.h"
#include "Object3D.h"
#include "Particle.h"
#include "Primitive.h"
#include "Camera.h"

// Manages all entites in a scene
class World
{
public:
	World();
	~World();

	void AddEntity(Entity* entity);
	bool DeleteEntity(std::string name);

	void Update();

	bool SetActiveCamera(std::string cameraName);
	Camera* GetActiveCamera();


private:

	std::map<std::string, Entity*> _entities;	// entity pool managed by the world

	std::map<std::string, Camera*> _cameras;	// camera entites in the scene
	Camera* _currentActiveCamera = nullptr;		// currently active camera

	std::map<std::string, Entity*> _renderBase;		// base render entites
};

