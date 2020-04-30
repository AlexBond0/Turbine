#include "Entity.h"



Entity::Entity(std::string name, EntityType type) {

	_name = name;
	_type = type;
}


Entity::Entity(Entity* copy)
	: MoveableUnit((MoveableUnit)copy)
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