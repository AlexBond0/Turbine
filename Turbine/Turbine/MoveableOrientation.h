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
	void SetOrientation(glm::vec3 rot);
	void SetOrientation(glm::quat rot);

	glm::vec3 GetOrientationEuler();
	glm::quat GetOrientationQuat();
	glm::mat4 GetOrientationMatrix();

	void SetRotation(float x, float y, float z, bool usingRads = false);
	void SetRotation(glm::vec3 rot);
	void SetRotation(glm::quat rot);
	void ShiftRotation(float* rot);
	void ShiftRotation(float x, float y, float z);

	glm::quat GetRotationQuat();
	glm::mat4 GetRotationMatrix();

protected:
	
	glm::quat _orientation;	// the base orientation of the element
	glm::quat _rotation;	// the applied rotation of the element
};

