#pragma once

#include <vector>
#include "Moveable.h"
#include "RenderingContext.h"

// Manages positional heirarchy of elements in a scene
class Entity
	: public Moveable
{
public:
	Entity();
	Entity(Entity* copy);
	~Entity();

	std::vector<Entity*> children;	// children in Entity heirarchy
	Entity* parent;					// parent in Entity heirarchy
	bool isRenderable = false;		// can the elemebt be rendered (Needs a better implementation)


	void AddChild(Entity* newChild);
	void SetParent(Entity* newParent);
	std::vector<Entity*> GetChildren();

	TranslationStack GetWorldTranslation();

protected:
	void _GetWorldTranslation(TranslationStack& translations);

};

