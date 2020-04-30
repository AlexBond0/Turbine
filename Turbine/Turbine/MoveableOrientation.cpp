#include "MoveableOrientation.h"



MoveableOrientation::MoveableOrientation() {

	_orientation = glm::quat(glm::vec3(0.0));
	_rotation = glm::quat(glm::vec3(0.0));

	_target = glm::vec3(0.0);
	_front = glm::vec3(0.0, 0.0, 1.0);
}


MoveableOrientation::MoveableOrientation(MoveableOrientation* copy) {

	_orientation = copy->_orientation;
	_rotation = copy->_rotation;

	_target = copy->_target;
	_front = copy->_front;
}

// ======================================

// Set the rotation of the element
void MoveableOrientation::SetOrientation(byte* buffer) {

	_orientation = glm::quat(glm::vec3(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8)
	));
}

// Set the rotation of the element
void MoveableOrientation::SetOrientation(float* rot) {

	_orientation = glm::quat(glm::vec3(
		rot[0],
		rot[1],
		rot[2]
	));
}

// Set the rotation of the element
void MoveableOrientation::SetOrientation(float x, float y, float z, bool usingRads) {

	if (usingRads) {

		_orientation = glm::quat(glm::vec3(
			x,
			y,
			z
		));
	}
	else {

		_orientation = glm::quat(glm::vec3(
			DEGSTORADS(x),
			DEGSTORADS(y),
			DEGSTORADS(z)
		));
	}

}

// Set the rotation of the element
void MoveableOrientation::SetOrientation(glm::quat rot) {

	_orientation = rot;
}

glm::quat MoveableOrientation::GetOrientationQuat() {

	return _orientation;
}

// Set the rotation of the element
void MoveableOrientation::SetRotation(float x, float y, float z, bool usingRads) {

	if (usingRads) {

		_rotation = glm::quat(glm::vec3(
			x,
			y,
			z
		));
	}
	else {

		_rotation = glm::quat(glm::vec3(
			DEGSTORADS(x),
			DEGSTORADS(y),
			DEGSTORADS(z)
		));
	}
}

// Set the rotation of the element
void MoveableOrientation::SetRotation(glm::quat rot) {

	_rotation = rot;
}


// =====================================================================

// Rotate the current rotation of the element
void MoveableOrientation::ShiftRotation(float* rot) {

	glm::quat quatRot = glm::quat(glm::vec3(
		DEGSTORADS(rot[0]),
		DEGSTORADS(rot[1]),
		DEGSTORADS(rot[2])
	));

	_rotation += quatRot;
}

// Rotate the current rotation of the element
void MoveableOrientation::ShiftRotation(float x, float y, float z) {

	glm::quat quatRot = glm::quat(glm::vec3(
		DEGSTORADS(x),
		DEGSTORADS(y),
		DEGSTORADS(z)
	));

	_rotation = glm::normalize(_rotation * quatRot);
}

glm::quat MoveableOrientation::GetRotationQuat() {

	return _rotation;
}


// =====================================================================

void MoveableOrientation::SetTarget(glm::vec3 target) {

	_target = target;
}

glm::vec3* MoveableOrientation::GetTarget() {

	return &_target;
}

void MoveableOrientation::SetFront(glm::vec3 front) {

	_front = front;
}

glm::vec3* MoveableOrientation::GetFront() {

	return &_front;
}