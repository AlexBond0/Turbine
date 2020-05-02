#pragma once

#include "Entity.h"
#include "Particle.h"
#include "ModelLoader.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	Entity* GetEntity(std::string name);
	std::map<std::string, Entity*> GetAllEntities();

	Object3D* CreateObject3D(std::string name);
	Object3D* DuplicateObject3D(std::string copyObjectName, std::string newObjectName);
	Object3D* GetObject3D(std::string name);

	void UnpackModel3D(ModelLoader* model);

	virtual void AddEntity(Entity* entity);
	virtual bool DeleteEntity(std::string name);
	virtual void Clean();
	virtual void Render(RenderingContext& rcontext) = 0;

protected:

	std::map<std::string, Entity*> _entities;	// entity pool managed by the EntityManager
	std::map<std::string, Particle*> _particleSystems;	// particle systems
};

