#include "Entity.h"



Entity::Entity(std::string name) {

	_name = name;
}


Entity::Entity(Entity* copy)
	: Moveable((Moveable)copy)
{
	children = copy->children;
	parent = copy->parent;
	_name = copy->_name;
}


Entity::~Entity()
{
}

// add a child to the object heirarchy
void Entity::AddChild(Entity* newChild) {

	children.push_back(newChild);

	glm::vec3 childPosition = newChild->GetWorldPosVec();

	newChild->SetLocalPos(newChild->GetWorldPosVec() - worldPos);

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
	translations.Translate(localPos[0], localPos[1], localPos[2]);
	translations.Translate(translation[0], translation[1], translation[2]);
	translations.Scale(scale[0], scale[1], scale[2]);

	// preform the rotation
	translations.Rotate(GetOrientationMatrix());
	translations.Rotate(GetRotationMatrix());
}

// Set the name of the entity
void Entity::SetName(const char* name) {
//{
//	const int len = strlen(name) + 1;
//	this->name = (char*)realloc(this->name, len);
//	strcpy_s(this->name, len, name);

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