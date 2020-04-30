#pragma once

#include "MoveablePoint.h"
#include "MoveableOrientation.h"

// a moveable system comprised of a point ion space and an orientation
class Moveable
	: public MoveablePoint
	, public MoveableOrientation
{
public:

	Moveable();
	Moveable(Moveable* copy);
	~Moveable() {};

	void PointAt(glm::vec3 newVector);

	void LookAt(glm::vec3 lookAt);
	void LookAtTarget();

	void Move(glm::vec3 direction);

protected:

	glm::quat _RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
};
