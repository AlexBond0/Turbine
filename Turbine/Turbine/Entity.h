#pragma once

#include <vector>
#include "Moveable.h"
#include "RenderingContext.h"

// Manages positional heirarchy of elements in a scene
class Entity
	: public Moveable
{
public:
	Entity(std::string name);
	Entity(Entity* copy);
	~Entity();

	std::vector<Entity*> children;	// children in Entity heirarchy
	Entity* parent;					// parent in Entity heirarchy

	// implement in child classes to define functionality of entity when called to render
	virtual void OnRender(RenderingContext& rcontext) = 0;

	void AddChild(Entity* newChild);
	void SetParent(Entity* newParent);
	std::vector<Entity*> GetChildren();

	std::string GetName();
	void SetName(const char* name);
	void SetName(std::string name);

	TranslationStack GetWorldTranslation();

protected:

	void _GetWorldTranslation(TranslationStack& translations);

	std::string _name;					// object name

};

