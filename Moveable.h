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
	//float* GetWorldPos();
	glm::vec3 GetWorldPosVec();

	void SetLocalPos(byte* buffer);
	void SetLocalPos(float x, float y, float z);
	//float* GetLocalPos();
	glm::vec3 GetLocalPosVec();

	void SetTranslation(byte* buffer);
	void SetTranslation(float x, float y, float z);
	//float* GetTranlation();
	glm::vec3 GetTranlationVec();

	void SetRotation(byte* buffer);
	void SetRotation(float* rot);
	void SetRotation(float x, float y, float z);
	//float* GetRotation();
	glm::vec3 GetRotationVec();

	void ShiftRotation(float* rot);
	void ShiftRotation(float x, float y, float z);

	void SetScale(byte* buffer);
	void SetScale(float x, float y, float z);
	//float* GetScale();
	glm::vec3 GetScaleVec();

protected:

	glm::vec3 worldPos;
	glm::vec3 localPos;
	
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;


	//float worldPos[3];
	//float localPos[3];

	//float translation[3];
	//float rotation[3];
	//float scale[3];
};



//inline float* Moveable::GetWorldPos() {
//
//	// return worldPos;
//
//	float v[3] = { worldPos.x, worldPos.y, worldPos.z };
//	return v;
//}

inline glm::vec3 Moveable::GetWorldPosVec() {

	return worldPos;
}



//inline float* Moveable::GetTranlation() {
//
//	// return translation;
//
//	float v[3] = { translation.x, translation.y, translation.z };
//	return v;
//}

inline glm::vec3 Moveable::GetTranlationVec() {

	return translation;
}



//inline float* Moveable::GetRotation() {
//
//	// return rotation;
//	
//	float v[3] = { rotation.x, rotation.y, rotation.z };
//	return v;
//}

inline glm::vec3 Moveable::GetRotationVec() {

	return rotation;
}



//inline float* Moveable::GetLocalPos() {
//
//	// return localPos;
//	
//	float v[3] = { localPos.x, localPos.y, localPos.z };
//	return v;
//}

inline glm::vec3 Moveable::GetLocalPosVec() {

	return localPos;
}



//inline float* Moveable::GetScale() {
//
//	// return scale;
//	
//	float v[3] = { scale.x, scale.y, scale.z };
//	return v;
//}

inline glm::vec3 Moveable::GetScaleVec() {

	return scale;
}

//#pragma once
//
//#include <Windows.h>
//
//
//class Moveable
//{
//public:
//	Moveable();
//	Moveable(Moveable* copy);
//	~Moveable();
//
//	void SetWorldPos(byte* buffer);
//	float* GetWorldPos();
//
//	void SetTranslation(byte* buffer);
//	void SetTranslation(float x, float y, float z);
//	float* GetTranlation();
//
//	void SetRotation(byte* buffer);
//	void SetRotation(float* rot);
//	void SetRotation(float x, float y, float z);
//	float* GetRotation();
//
//	void ShiftRotation(float* rot);
//	void ShiftRotation(float x, float y, float z);
//
//	void SetScale(byte* buffer);
//	void SetScale(float x, float y, float z);
//	float* GetScale();
//
//	void SetLocalPos(byte* buffer);
//	void SetLocalPos(float x, float y, float z);
//	float* GetLocalPos();
//
//protected:
//
//	float worldPos[3];
//	float translation[3];
//	float localPos[3];
//	float rotation[3];
//	float scale[3];
//};
//
//inline float* Moveable::GetWorldPos() {
//
//	return worldPos;
//}
//
//inline float* Moveable::GetTranlation() {
//
//	return translation;
//}
//
//inline float* Moveable::GetRotation() {
//
//	return rotation;
//}
//
//inline float* Moveable::GetLocalPos() {
//
//	return localPos;
//}
//
//inline float* Moveable::GetScale() {
//
//	return scale;
//}