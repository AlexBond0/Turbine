#pragma once

#include <Windows.h>


class Moveable
{
public:
	Moveable();
	Moveable(Moveable* copy);
	~Moveable();

	void SetWorldPos(byte* buffer);
	float* GetWorldPos();

	void SetTranslation(byte* buffer);
	void SetTranslation(float x, float y, float z);
	float* GetTranlation();

	void SetRotation(byte* buffer);
	void SetRotation(float* rot);
	void SetRotation(float x, float y, float z);
	float* GetRotation();

	void ShiftRotation(float* rot);
	void ShiftRotation(float x, float y, float z);

	void SetScale(byte* buffer);
	void SetScale(float x, float y, float z);
	float* GetScale();

	void SetLocalPos(byte* buffer);
	void SetLocalPos(float x, float y, float z);
	float* GetOffset();

protected:

	float worldPos[3];
	float localPos[3];
	
	float translation[3];
	float rotation[3];
	float scale[3];
};

inline float* Moveable::GetWorldPos() {

	return worldPos;
}

inline float* Moveable::GetTranlation() {

	return translation;
}

inline float* Moveable::GetRotation() {

	return rotation;
}

inline float* Moveable::GetOffset() {

	return localPos;
}

inline float* Moveable::GetScale() {

	return scale;
}