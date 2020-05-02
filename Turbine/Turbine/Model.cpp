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

	for (auto const& baseEntity : _baseEntites)
		baseEntity.second->OnRender(rcontext);
}