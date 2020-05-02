#include "Model.h"

Model::Model(std::string name) 
	: Entity(name, EntityType::MODEL)
{

}


Entity* Model::GetBaseEntity() {

	return _baseEntity;
}

bool Model::SetBaseEntity(std::string name) {

	bool found = false;

	// base entity exists
	if (_entities.count(name) > 0) {

		_baseEntity = _entities[name];
		found = true;
	}

	return found;
}

void Model::Render(RenderingContext& rcontext) {

	_baseEntity->OnRender(rcontext);
}