#pragma once

#include "Matrix.h"
#include <windows.h>

class MoveableOrientation
{
public:
	MoveableOrientation();
	MoveableOrientation(MoveableOrientation* copy);
	~MoveableOrientation() {};

	void SetOrientation(byte* buffer);
	void SetOrientation(float* rot);
	void SetOrientation(float x, float y, float z, bool usingRads = false);
	void SetOrientation(glm::quat rot);
	glm::quat GetOrientationQuat();

	void SetRotation(float x, float y, float z, bool usingRads = false);
	void SetRotation(glm::quat rot);
	void ShiftRotation(float* rot);
	void ShiftRotation(float x, float y, float z);
	glm::quat GetRotationQuat();

	void SetTarget(glm::vec3 target);
	glm::vec3* GetTarget();

	void SetFront(glm::vec3 front);
	glm::vec3* GetFront();

	bool useTarget = false;

protected:
	
	glm::vec3 _target;
	glm::vec3 _front;

	glm::quat _orientation;	// the base orientation of the element
	glm::quat _rotation;	// the applied rotation of the element
};

