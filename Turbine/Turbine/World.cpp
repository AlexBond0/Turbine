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

		_currentActiveCamera->fDirty = true;
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

std::string World::Serialize() {

	json data;

	// serialize all tracked entites 
	json wd;
	for (const auto& e : _entities) 
		wd[e.second->GetName()] = e.second->Serialize();

	data["World"] = wd;

	// create map for relationshipping
	json rs;
	_CalculateRelations(rs, _entities);
	data["Relations"] = rs;

	// save Environment
	data["Environment"] = enviro.Serialize();

	data["CurrentCamera"] = GetActiveCamera()->GetName();

	return data.dump();
}

void World::_CalculateRelations(json& data, std::map<std::string, Entity*> entites) {

	Entity* parent;
	Entity* me;

	for (const auto& e : entites) {

		// check if entity is a collection of other entites
		if (e.second->GetEntityType() == EntityType::MODEL) {

			Model* model = dynamic_cast<Model*>(e.second);
			_CalculateRelations(data, model->GetAllEntities());
		}

		// normal entity
		else {

			me = e.second;
			parent = e.second->parent;

			json meData;

			// add model name first 
			if (_HasEntityBeenLoaded(me))
				meData["child"].push_back(_GetModelName(me));

			// add entity name
			meData["child"].push_back(me->GetName());


			// is there a parent
			if (parent != nullptr) {

				// add model name first 
				if (_HasEntityBeenLoaded(parent))
					meData["parent"].push_back(_GetModelName(parent));

				// add entity name
				meData["parent"].push_back(parent->GetName());
			}

			// no parent = empty array
			else {

				meData["parent"] = json::array();
			}

			data.push_back(meData);
		}
	}
}

bool World::_HasEntityBeenLoaded(Entity* entity) {

	Object3D* objEntity = dynamic_cast<Object3D*>(entity);
	return (objEntity != nullptr && objEntity->loadedFrom.compare("") != 0);
}

std::string World::_GetModelName(Entity* entity) {

	Object3D* objEntity = dynamic_cast<Object3D*>(entity);
	if (objEntity != nullptr && objEntity->loadedFrom.compare("") != 0) {

		return objEntity->loadedFrom;
	}
	else {

		return "";
	}
}

World* World::Deserialize(json& data) {

	World* world = new World();
	json entity;
	std::string entityName;
	ModelLoader* model;

	// =======================================================================
	// Load object files

	for (auto& el : data["World"].items()) {

		entity = el.value();
		entityName = el.key();

		if (entity.find("Model") != entity.end()) {

			model = ModelLoader::LoadModel(entity["Model"]["fileName"]);
			world->AddEntity(model->GetModel());
		}
	}

	// =======================================================================
	// Create Entities

	Entity* entityPointer = nullptr;
	for (auto& el : data["World"].items()) {

		entity = el.value();
		entityName = el.key();

		// get element type
		// there will only ever be one 
		std::string elementType;
		for (auto& type : entity.items())
			elementType = type.key();
 

		switch (Entity::ConvertEntityType(elementType)) {

			case EntityType::OBJ: {

				entityPointer = new Object3D(entity["Object3D"]);
				break;
			}

			case EntityType::MODEL: {

				// models allready loaded via object files
				entityPointer = nullptr;
				break;
			}
		
			case EntityType::OBJ_INSTANCED: {

				entityPointer = new InstancedObject(entity["InstancedObject"]);
				break;
			}

			
			case EntityType::OBJ_PARTICLE_SYS: {

				entityPointer = new Particle(entity["Particle"]);
				break;
			}

			case EntityType::CAMERA: {

				entityPointer = new Camera(entity["Camera"]);
				break;
			}
			
			case EntityType::LIGHT: {

				if (elementType.compare("PointLight") == 0)
					entityPointer = new PointLight(entity["PointLight"]);

				else if (elementType.compare("DirectionalLight") == 0)
					entityPointer = new DirectionalLight(entity["DirectionalLight"]);

				else if (elementType.compare("SpotLight") == 0)
					entityPointer = new SpotLight(entity["SpotLight"]);

				break;
			}

			case EntityType::EMPTY: {

				entityPointer = new Entity(entity);
				entityPointer->SetEntityType(EntityType::EMPTY);
				break;
			}

		}

		if (entityPointer != nullptr) {

			entityPointer->SetName(entityName);
			world->AddEntity(entityPointer);
		}
	}

	// =======================================================================
	// Find relationships

	Entity* child;
	Entity* parent;

	for (auto& relation : data["Relations"]) {

		// get child
		if (relation["child"].size() == 1) {

			child = world->GetEntity(relation["child"].at(0));
		}
		else if (relation["child"].size() == 2) {

			child = world->GetModelEntity(relation["child"].at(0), relation["child"].at(1));
		}
		else {

			child = nullptr;
		}

		// get parent
		if (relation["parent"].size() == 1) {

			parent = world->GetEntity(relation["parent"].at(0));
		}
		else if (relation["parent"].size() == 2) {

			parent = world->GetModelEntity(relation["parent"].at(0), relation["parent"].at(1));
		}
		else {

			parent = nullptr;
		}

		if (child != nullptr && parent != nullptr) {

			child->SetParent(parent);
			parent->AddChild(child);
		}
	}

	// =======================================================================
	// Extra Data

	world->enviro = Environment(data["Environment"]);

	world->SetActiveCamera(data["CurrentCamera"]);

	return world;
}
