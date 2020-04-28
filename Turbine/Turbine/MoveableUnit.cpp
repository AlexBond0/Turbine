#include "MoveableUnit.h"

MoveableUnit::MoveableUnit() {

}

MoveableUnit::MoveableUnit(MoveableUnit* copy)
	: MoveablePoint(MoveablePoint(copy))
	, MoveableOrientation(MoveableOrientation(copy))
{

}

// point towards a vector from a given starting vector direction
void MoveableUnit::PointAt(glm::vec3 newVector) {

	newVector = glm::normalize(newVector);
	_orientation = _RotationBetweenVectors(_up, newVector);
}

// Returns a quaternion such that q*start = dest
glm::quat MoveableUnit::_RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {

		// special case when vectors in opposite directions :
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		// This implementation favors a rotation around the Up axis,
		// since it's often what you want to do.
		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return angleAxis(glm::radians(180.0f), rotationAxis);
	}

	// Implementation from Stan Melax's Game Programming Gems 1 article
	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}