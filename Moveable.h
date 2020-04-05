#pragma once

#include <Windows.h>
#include "Matrix.h"


class Moveable
{
public:
	Moveable();
	Moveable(Moveable* copy);
	~Moveable();

	void SetWorldPos(byte* buffer);
	glm::vec3 GetWorldPosVec();

	void SetLocalPos(byte* buffer);
	void SetLocalPos(float x, float y, float z);
	glm::vec3 GetLocalPosVec();

	void SetTranslation(byte* buffer);
	void SetTranslation(float x, float y, float z);
	glm::vec3 GetTranlationVec();

	void SetRotation(byte* buffer);
	void SetRotation(float* rot);
	void SetRotation(float x, float y, float z);
	glm::vec3 GetRotationVec();

	void ShiftRotation(float* rot);
	void ShiftRotation(float x, float y, float z);

	void SetScale(byte* buffer);
	void SetScale(float x, float y, float z);
	glm::vec3 GetScaleVec();

protected:

	glm::vec3 worldPos;
	glm::vec3 localPos;
	
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;
};


inline glm::vec3 Moveable::GetWorldPosVec() {

	return worldPos;
}

inline glm::vec3 Moveable::GetTranlationVec() {

	return translation;
}

inline glm::vec3 Moveable::GetRotationVec() {

	return rotation;
}

inline glm::vec3 Moveable::GetLocalPosVec() {

	return localPos;
}

inline glm::vec3 Moveable::GetScaleVec() {

	return scale;
}