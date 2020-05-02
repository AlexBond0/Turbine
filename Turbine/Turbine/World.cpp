#include "World.h"


// Add an existsing entity to the world
// entites managed by the world will be deleted by the world
void World::AddEntity(Entity* entity) {

	EntityManager::AddEntity(entity);

	// manage entity type
	switch (entity->GetEntityType()) {

		case EntityType::MODEL: {

			Model* model = dynamic_cast<Model*>(entity);

			_models[model->GetName()] = model;
			break;
		}

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

Entity* World::GetModelEntity(std::string model, std::string name) {

	return _models[model]->GetEntity(name);
}

Object3D* World::GetModelObject3D(std::string model, std::string name) {

	return dynamic_cast<Object3D*>(_models[model]->GetEntity(name));
}