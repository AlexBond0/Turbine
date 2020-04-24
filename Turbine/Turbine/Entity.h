#pragma once

#include <vector>
#include "Moveable.h"
#include "RenderingContext.h"

// type of entity
enum class EntityType {

	OBJ,				// Basic Object3D
	OBJ_PRIMITIVE,		// Primitive object
	OBJ_INSTANCED,		// Insanced Object3D
	OBJ_PARTICLE_SYS,	// Object Particle system
	LIGHT,				// Light
	CAMERA				// Camera
};

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

	// implement in child classes to define functionality of entity when called to render
	virtual void OnRender(RenderingContext& rcontext) = 0;

	// implement in child classes to define the Object3D* data to pick against
	virtual Entity* OnPick() = 0;


	void AddChild(Entity* newChild);
	void SetParent(Entity* newParent);
	std::vector<Entity*> GetChildren();

	std::string GetName();
	void SetName(const char* name);
	void SetName(std::string name);

	TranslationStack GetWorldTranslation();

	EntityType GetEntityType();
	void SetEntityType(EntityType type);

	void RemoveFromWorld();
	bool FlaggedForRemoval();

protected:

	void _GetWorldTranslation(TranslationStack& translations);

	std::string _name;					// object name

private:

	EntityType _type;	// type of entity
	bool _removeFromWorld = false;
};

