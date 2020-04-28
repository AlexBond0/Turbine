#pragma once

#include "MoveablePoint.h"
#include "MoveableOrientation.h"

// a moveable system comprised of a point ion space and an orientation
class MoveableUnit
	: public MoveablePoint
	, public MoveableOrientation
{
public:

	MoveableUnit();
	MoveableUnit(MoveableUnit* copy);
	~MoveableUnit() {};

	void PointAt(glm::vec3 newVector);

protected:

	glm::quat _RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);
};

