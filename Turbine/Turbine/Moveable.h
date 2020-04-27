#pragma once

#include <Windows.h>
#include "Matrix.h"

// Inherit from this class to include simple spacial properties
class Moveable
{
public:
	Moveable();
	Moveable(Moveable* copy);
	~Moveable();

	void SetWorldPos(byte* buffer);
	void SetWorldPos(glm::vec3 pos);
	glm::vec3 GetWorldPosVec();

	void SetLocalPos(byte* buffer);
	void SetLocalPos(float x, float y, float z);
	void SetLocalPos(glm::vec3 pos);
	glm::vec3 GetLocalPosVec();

	void SetTranslation(byte* buffer);
	void SetTranslation(float x, float y, float z);
	void SetTranslation(glm::vec3 pos);
	glm::vec3 GetTranlationVec();

	void SetUp(glm::vec3 direction);
	glm::vec3 GetUp();

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


	void SetScale(byte* buffer);
	void SetScale(float x, float y, float z);
	void SetScale(glm::vec3 newScale);
	glm::vec3 GetScaleVec();

	void PointAt(glm::vec3 newVector);

protected:

	glm::quat _RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

	glm::vec3 worldPos;
	glm::vec3 localPos;
	
	glm::vec3 translation;
	glm::vec3 scale;

	glm::vec3 up;

	glm::quat rotation;		// the rotation applied to the object
	glm::quat orientation;	// the base orientation of the object
};


inline glm::vec3 Moveable::GetWorldPosVec() {

	return worldPos;
}

inline glm::vec3 Moveable::GetTranlationVec() {

	return translation;
}

inline glm::vec3 Moveable::GetOrientationEuler() {

	// return rotation;
	return glm::eulerAngles(orientation);
}

inline glm::quat Moveable::GetOrientationQuat() {

	return orientation;
}

inline glm::mat4 Moveable::GetOrientationMatrix() {

	return glm::toMat4(orientation);
}

inline glm::quat Moveable::GetRotationQuat() {

	return rotation;
}

inline glm::mat4 Moveable::GetRotationMatrix() {

	return glm::toMat4(rotation);
}

inline glm::vec3 Moveable::GetLocalPosVec() {

	return localPos;
}

inline glm::vec3 Moveable::GetScaleVec() {

	return scale;
}