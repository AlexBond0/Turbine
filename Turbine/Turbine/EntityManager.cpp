#include "EntityManager.h"



EntityManager::~EntityManager() {

	// delete all entities managed by the entity manager
	for (auto const& entity : _entities)
		if (entity.second)
			delete entity.second;
}

// Return a pointer to an entity given the name of the entity
Entity* EntityManager::GetEntity(std::string name) {

	return _entities[name];
}

// return map of all managed entities
std::map<std::string, Entity*> EntityManager::GetAllEntities() {

	return _entities;
}

std::map<std::string, Entity*> EntityManager::GetAllBaseEntities() {

	return _baseEntites;
}

// The ammount of entities currently managed
int EntityManager::GetEntityCount() {

	return _entities.size();
}

// Create an Object3D in the entity manager
Object3D* EntityManager::CreateObject3D(std::string name) {

	Object3D* newObj = new Object3D(name);
	AddEntity(newObj);
	return newObj;
}

// Duplicate an Object3D in the entity manager and return the new object3D pointer
Object3D* EntityManager::DuplicateObject3D(Object3D* copyObject, std::string newObjectName, bool addToManager = true) {

	Object3D* newObj = new Object3D(copyObject, newObjectName);

	if (addToManager)
		AddEntity(newObj);

	return newObj;
}

// Return a pointer to a dynamically casted Object3D given the name of the entity
Object3D* EntityManager::GetObject3D(std::string name) {

	return dynamic_cast<Object3D*>(_entities[name]);
}


// ================================================================================

// Update all current particle systems in the entity manager
void EntityManager::UpdateParticles(double timePassed) {

	for (auto const& particleSys : _particleSystems)
		particleSys.second->Update(timePassed);
}

// Add an existsing entity to the entity manager
// entites managed by the entity manager will be deleted by the entity manager
void EntityManager::AddEntity(Entity* entity) {

	// add entity to the managed entity pool
	_entities[entity->GetName()] = entity;

	// if the object has no parent
	if (entity->parent == nullptr) {

		_baseEntites[entity->GetName()] = entity;
	}

	// manage entity type
	switch (entity->GetEntityType()) {

		case EntityType::OBJ:
		case EntityType::OBJ_INSTANCED:
		case EntityType::OBJ_PRIMITIVE:
		{


			break;
		}

		// save particle systems 
		case EntityType::OBJ_PARTICLE_SYS: {

			Particle* particle = dynamic_cast<Particle*>(entity);
			_particleSystems[particle->GetName()] = particle;

			break;
		}
	}
}

// Remove an entity form the entity manager, retruns true if found entity to remove
bool EntityManager::DeleteEntity(std::string name) {

	bool deleted = false;

	if (_entities.count(name) > 0) {

		Entity* entity = _entities[name];

		switch (entity->GetEntityType()) {

			case EntityType::OBJ: {

				delete entity;
				_entities.erase(name);

				deleted = true;
				break;
			}

			case EntityType::OBJ_PARTICLE_SYS: {

				// remove camera
				_particleSystems.erase(name);
				delete entity;

				deleted = true;
				break;
			}
		}

		return deleted;
	}
	else
		return deleted;
}

// Check all entities and update as necessary
void EntityManager::Clean() {

	// check all saved entities
	Entity* entity;
	std::string entityName;

	for (auto const& entityPair : _entities) {

		entity = entityPair.second;
		entityName = entity->GetName();

		// does entity need deleting
		if (entity->FlaggedForRemoval()) {

			DeleteEntity(entityName);
		}

		// does entity needs checking
		else if (entity->isDirty) {

			// no longer dirty
			entity->isDirty = false;
		}

		// is entity still a base entity
		if (entity->parent == nullptr) {

			_baseEntites[entityName] = entity;
		}
		else {

			_baseEntites.erase(entityName);
		}
	}
}

// Render the entites in the world using a RenderingContext
void EntityManager::Render(RenderingContext& rcontext) {

	for (auto const& baseEntity : _baseEntites)
		baseEntity.second->OnRender(rcontext);
}