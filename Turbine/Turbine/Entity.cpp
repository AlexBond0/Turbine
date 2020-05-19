#include "Entity.h"



Entity::Entity(std::string name, EntityType type) {

	_name = name;
	_type = type;
}


Entity::Entity(Entity* copy)
	: Moveable((Moveable)copy)
{
	children = copy->children;
	parent = copy->parent;
	_name = copy->_name;
	_type = copy->_type;
}


Entity::~Entity()
{
}

// add a child to the object heirarchy
void Entity::AddChild(Entity* newChild) {

	children.push_back(newChild);

	glm::vec3 childPosition = *newChild->GetWorldPosVec();

	newChild->SetLocalPos(*newChild->GetWorldPosVec() - _worldPos);

	newChild->SetParent(this);
};

// add a child to the object heirarchy
void Entity::SetParent(Entity* newParent) {

	parent = newParent;
};

// Get vector of child objects 
std::vector<Entity*> Entity::GetChildren() {

	return children;
}

// Get a TranslationStack representing translation of object for current world space
TranslationStack Entity::GetWorldTranslation() {

	TranslationStack translations;
	translations.InitModelMatrix(true);

	_GetWorldTranslation(translations);

	return(translations);
}

// Recursively build a TranslationStack from object parents
void Entity::_GetWorldTranslation(TranslationStack& translations) {

	// do parent translation first
	if (parent != nullptr)
		parent->_GetWorldTranslation(translations);

	// save current model matrix
	translations.PushModelMatrix();

	// as the current translation values are global and not local, preform the offset values instead
	translations.Translate(_localPos);
	translations.Translate(_translation);
	translations.Scale(_scale);

	// preform the rotation
	translations.Rotate(glm::toMat4(_orientation));
	translations.Rotate(glm::toMat4(_rotation));
}

glm::vec3 Entity::GetWorldPosition() {

	TranslationStack translations;
	translations.InitModelMatrix(true);

	_GetWorldTranslation(translations);

	glm::mat4 transformation = *translations.GetCurrentModelMatrix();

	return(glm::vec3(transformation[3]));
	// return(glm::vec4(_localPos, 1.0) * transformation);
	// return _localPos;
}

// Set the name of the entity
void Entity::SetName(const char* name) {

	_name = std::string(name);
}

// Set the name of the entity
void Entity::SetName(std::string name) {

	_name = name;
}

// Get name of entity
std::string Entity::GetName() {

	return _name;
}

// Get the type of entity
EntityType Entity::GetEntityType() {

	return _type;
}

// Set the entity type, can result in unexpected
// results when not used safely
void Entity::SetEntityType(EntityType type) {

	_type = type;
}

// Delete the entity & remove it from the world
void Entity::RemoveFromWorld() {

	_removeFromWorld = true;
}

// Is the entity flagged for removal from the world
bool Entity::FlaggedForRemoval() {

	return _removeFromWorld;
}

json Entity::Serialize() {

	json me = Moveable::Serialize();

	for (Entity* e : children) 
		me["children"].push_back(e->GetName());
	

	if (parent != nullptr)
		me["parent"] = parent->GetName();

	me["isDirty"] = isDirty;
	me["isActive"] = isActive;
	me["isLocallyActive"] = isLocallyActive;

	me["type"] = _type;

	// pack and send json
	json ret;
	ret["Entity"] = me;
	return ret;
}

// Deserialize
Entity::Entity(json& data)
	: Moveable(data["Moveable"]) {

	isDirty = data["isDirty"];
	isActive = data["isActive"];
	isLocallyActive = data["isLocallyActive"];

	_type = data["type"];
}

std::string Entity::ConvertEntityType(EntityType type) {

	switch (type) {

		case EntityType::OBJ: return "Object3D";
		case EntityType::MODEL: return "Model";
		case EntityType::OBJ_INSTANCED: return "Instanced";
		case EntityType::OBJ_PARTICLE_SYS: return "Particle";
		case EntityType::OBJ_PRIMITIVE: return "Primitive";
		case EntityType::CAMERA: return "Camera";
		case EntityType::LIGHT: return "Light";
		case EntityType::EMPTY: return "Empty";
	}

	return "Unknown!";
}

EntityType Entity::ConvertEntityType(std::string type) {

	if (type.compare("Object3D") == 0)
		return EntityType::OBJ;

	else if (type.compare("Model") == 0)
		return EntityType::MODEL;

	else if (type.compare("Instanced") == 0)
		return EntityType::OBJ_INSTANCED;

	else if (type.compare("Particle") == 0)
		return EntityType::OBJ_PARTICLE_SYS;

	else if (type.compare("Primitive") == 0)
		return EntityType::OBJ_PRIMITIVE;

	else if (type.compare("Camera") == 0)
		return EntityType::CAMERA;

	else if (type.compare("Light") == 0)
		return EntityType::LIGHT;

	else if (type.compare("PointLight") == 0)
		return EntityType::LIGHT;

	else if (type.compare("DirectionalLight") == 0)
		return EntityType::LIGHT;

	else if (type.compare("SpotLight") == 0)
		return EntityType::LIGHT;

	else
		return EntityType::EMPTY;
}