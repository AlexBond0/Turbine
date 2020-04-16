#pragma once

#include <vector>
#include "Moveable.h"
#include "RenderingContext.h"


class Entity
	: public Moveable
{
public:
	Entity();
	Entity(Entity* copy);
	~Entity();

	std::vector<Entity*> children;	// children in Entity heirarchy
	Entity* parent;					// parent in Entity heirarchy

	void AddChild(Entity* newChild);
	void SetParent(Entity* newParent);
	std::vector<Entity*> GetChildren();

	TranslationStack GetWorldTranslation();

protected:
	void _GetWorldTranslation(TranslationStack& translations);

};

