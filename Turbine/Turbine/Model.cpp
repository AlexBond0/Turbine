#include "Model.h"

Model::Model(std::string name) 
	: Entity(name, EntityType::MODEL)
{

}


Object3D* Model::OnPick() {

	return nullptr;
}

// Check all entities and update as necessary

void Model::OnRender(RenderingContext& rcontext) {

	if (isActive && isLocallyActive) {

		// save current model matrix
		rcontext.PushModelMatrix();

		// as the current translation values are global and not local, preform the offset values instead
		rcontext.Translate(_localPos);
		rcontext.Translate(_translation);
		rcontext.Scale(_scale);

		// preform the rotation
		rcontext.Rotate(glm::toMat4(_orientation));
		rcontext.Rotate(glm::toMat4(_rotation));

		for (auto const& baseEntity : _baseEntites)
			baseEntity.second->OnRender(rcontext);

		rcontext.PopModelMatrix();
	}
}

// default clean, and assign base entity parent to model
void Model::Clean() {

	// Do default clean
	EntityManager::Clean();

	// check all saved entities
	for (auto const& entityPair : _entities) {

		// is entity still a base entity
		if (entityPair.second->parent == nullptr)
			entityPair.second->parent = this;
	}
}

json Model::Serialize() {

	json me = Entity::Serialize();
	me["fileName"] = filename;

	// all contained elements
	for (const auto& e : _entities)
		me[e.second->GetName()] = e.second->Serialize();
	
	// pack and send json
	json ret;
	ret["Model"] = me;
	return ret;
}