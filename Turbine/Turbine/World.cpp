#include "World.h"



World::World()
{
}


World::~World() {

	// delete all entities managed by the world
	//for (auto const& entity : _entities)
	//	delete entity.second;
}


// Add an existsing entity to the world
// entites managed by the world will be deleted by the world
void World::AddEntity(Entity* entity) {

	EntityManager::AddEntity(entity);

	// add entity to the managed entity pool
	// _entities[entity->GetName()] = entity;

	// if the object has no parent
	if (entity->parent == nullptr) {

		_renderBase[entity->GetName()] = entity;
	}

	// manage entity type
	switch (entity->GetEntityType()) {

		//case EntityType::OBJ:
		//case EntityType::OBJ_INSTANCED:
		//case EntityType::OBJ_PRIMITIVE: 
		//{


		//	break;
		//}

		//// save particle systems 
		//case EntityType::OBJ_PARTICLE_SYS: {

		//	Particle* particle = dynamic_cast<Particle*>(entity);
		//	_particleSystems[particle->GetName()] = particle;

		//	break;
		//}

		// save the camera to the scene
		case EntityType::CAMERA: {

			Camera* cam = dynamic_cast<Camera*>(entity);

			_cameras[cam->GetName()] = cam;

			if (_currentActiveCamera == nullptr)
				_currentActiveCamera = cam;

			break;
		}

		case EntityType::LIGHT: {

			Light* light = dynamic_cast<Light*>(entity);

			lights.AddLight(light);
		}
	}
}

// Return a pointer to an entity given the name of the entity
//Entity* World::GetEntity(std::string name) {
//
//	return _entities[name];
//}

// Remove an entity form the world, retruns true if found entity to remove
bool World::DeleteEntity(std::string name) {

	// if the entitymanager didn't delete the entity
	if (!EntityManager::DeleteEntity(name)) {

		bool deleted = false;

		if (_entities.count(name) > 0) {

			Entity* entity = _entities[name];

			switch (entity->GetEntityType()) {

				case EntityType::CAMERA: {

					// remove camera
					_cameras.erase(name);
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
	else
		return true;
}

// Create an Object3D in the world
//Object3D* World::CreateObject3D(std::string name) {
//
//	Object3D* newObj = new Object3D(name);
//	AddEntity(newObj);
//	return newObj;
//}
//
//// Duplicate an Object3D in the scene and return the new object3D pointer
//Object3D* World::DuplicateObject3D(std::string copyObjectName, std::string newObjectName) {
//
//	Object3D* oldObj = GetObject3D(copyObjectName);
//	Object3D* newObj = new Object3D(oldObj, newObjectName);
//	AddEntity(newObj);
//	return newObj;
//}
//
//// Return a pointer to a dynamically casted Object3D given the name of the entity
//Object3D* World::GetObject3D(std::string name) {
//
//	return dynamic_cast<Object3D*>(_entities[name]);
//}
//
//// Unpack entites loaded from Model3D into the entity manager
//void World::UnpackModel3D(ModelLoader* model) {
//
//	for (int i = 0; i < model->GetNoOfObjects(); i++) {
//
//		AddEntity(model->GetObjects()[i]);
//	}
//
//	Clean();
//}

// Check all entities and update as necessary
void World::Clean() {

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

			_renderBase[entityName] = entity;
		}
		else {

			_renderBase.erase(entityName);
		}
	}
}

// Render the entites in the world using a RenderingContext
void World::Render(RenderingContext& rcontext) {

	for (auto const& baseEntity : _renderBase)
		baseEntity.second->OnRender(rcontext);
}

// Set the world's current active camera, retruns true if found camera to activate
bool World::SetActiveCamera(std::string cameraName) {

	if (_cameras.count(cameraName) > 0) {

		_currentActiveCamera = _cameras[cameraName];
		return true;
	}
	else
		return false;
}

// Get the current active camera in the scene
Camera* World::GetActiveCamera() {

	return _currentActiveCamera;
}

// Update all current particle systems in the world
void World::UpdateParticles(double timePassed) {

	for (auto const& particleSys : _particleSystems)
		particleSys.second->Update(timePassed);
}