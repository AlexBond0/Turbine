#pragma once

#include "Particle.h"
#include "Entity.h"


// Manages a group of entites
class EntityManager {

public:
	EntityManager() {};
	~EntityManager();

	Entity* GetEntity(std::string name);
	std::map<std::string, Entity*> GetAllEntities();
	std::map<std::string, Entity*> GetAllBaseEntities();
	int GetEntityCount();

	Object3D* CreateObject3D(std::string name);
	Object3D* DuplicateObject3D(Object3D* copyObject, std::string newObjectName, bool addToManager);
	Object3D* GetObject3D(std::string name);


	// ==========================================

	void UpdateParticles(double timePassed);
	void AddEntity(Entity* entity);
	bool DeleteEntity(std::string name);
	void Clean();
	void Render(RenderingContext& rcontext);

protected:

	std::map<std::string, Entity*> _entities;			// entity pool managed by the EntityManager
	std::map<std::string, Entity*> _baseEntites;		// base  entites

	std::map<std::string, Particle*> _particleSystems;	// particle systems
};

