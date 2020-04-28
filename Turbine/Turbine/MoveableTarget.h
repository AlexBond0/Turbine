#pragma once

#include "Matrix.h"

class MoveableTarget
{
public:

	MoveableTarget();
	MoveableTarget(MoveableTarget* copy);
	~MoveableTarget() {};

	void SetTarget(glm::vec3 target);
	glm::vec3* GetTarget();

protected:

	glm::vec3 _target;
};

