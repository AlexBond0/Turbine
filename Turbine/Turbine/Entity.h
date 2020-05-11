#pragma once

#include <vector>
#include "Moveable.h"
#include "RenderingContext.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

// type of entity
enum class EntityType {

	OBJ,				// Basic Object3D
	OBJ_PRIMITIVE,		// Primitive object
	OBJ_INSTANCED,		// Insanced Object3D
	OBJ_PARTICLE_SYS,	// Object Particle system
	LIGHT,				// Light
	CAMERA,				// Camera
	MODEL				// Model system
};

class Object3D; // forward decleration

// Manages positional heirarchy of elements in a scene
class Entity
	: public Moveable
{
public:
	Entity(std::string name, EntityType type);
	Entity(Entity* copy);
	~Entity();

	std::vector<Entity*> children;	// children in Entity heirarchy
	Entity* parent;					// parent in Entity heirarchy

	bool isDirty = false;			// flag to tell world if entity needs evaluating
	bool isActive = true;			// does the object and object children get rendered
	bool isLocallyActive = true;	// does the object get rendered
	bool IsGloballyActive();		// is the object globally active in the scene

	// implement in child classes to define functionality of entity when called to render
	virtual void OnRender(RenderingContext& rcontext) = 0;

	// implement in child classes to define the Object3D* data to pick against
	virtual Object3D* OnPick() = 0;


	void AddChild(Entity* newChild);
	void SetParent(Entity* newParent);
	std::vector<Entity*> GetChildren();

	std::string GetName();
	void SetName(const char* name);
	void SetName(std::string name);

	TranslationStack GetWorldTranslation();
	glm::vec3 GetWorldPosition();

	EntityType GetEntityType();
	void SetEntityType(EntityType type);

	void RemoveFromWorld();
	bool FlaggedForRemoval();

	virtual void Serialize(json& data);
	virtual void Deserialize(json& data);

protected:

	void _GetWorldTranslation(TranslationStack& translations);

	std::string _name;	// entity name

private:

	EntityType _type;	// type of entity
	bool _removeFromWorld = false;
};

inline bool Entity::IsGloballyActive() {

	if (parent != nullptr)
		return (isActive && parent->IsGloballyActive());

	else
		return isActive;
}