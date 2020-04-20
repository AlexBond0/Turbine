#include "World.h"



World::World()
{
}


World::~World() {

	// delete all entities managed by the world
	for (auto const& entity : _entities)
		delete entity.second;
}


// Add an existsing entity to the world
// entites managed by the world will be deleted by the world
void World::AddEntity(Entity* entity) {

	// add entity to the managed entity pool
	_entities[entity->GetName()] = entity;

	// manage entity type
	switch (entity->GetEntityType()) {

		case EntityType::OBJ: {

			// Object3D* object = static_cast<Object3D*>(entity);

			// if the object has no parent
			if (entity->parent == nullptr) {

				_renderBase[entity->GetName()] = entity;
			}

			break;
		}

		// save the camera to the scene
		case EntityType::CAMERA: {

			Camera* cam = static_cast<Camera*>(entity);

			_cameras[cam->GetName()] = cam;

			if (_currentActiveCamera == nullptr)
				_currentActiveCamera = cam;

			break;
		}
	}
}

// Remove an entity form the world, retruns true if found entity to remove
bool World::DeleteEntity(std::string name) {

	if (_entities.count(name) > 0) {

		Entity* entity = _entities[name];

		switch (entity->GetEntityType()) {

			case EntityType::OBJ : {

				if (_renderBase.count(name) > 0)
					_renderBase.erase(name);

				delete entity;
				break;
			}

			case EntityType::CAMERA: {

				// remove camera
				_cameras.erase(name);
				delete entity;

				// if (_currentActiveCamera == nullptr)

				break;
			}

		}

		return true;
	}
	else
		return false;
}

void World::Update() {

	// check all saved entities
	Entity* entity;
	for (auto const& entityPair : _entities) {
		entity = entityPair.second;

		// does entity need deleting
		if (entity->FlaggedForRemoval()) {

		}

		// does entity needs checking
		else if (entity->isDirty) {

		}
	}
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