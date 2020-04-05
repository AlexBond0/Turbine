#include "Moveable.h"



Moveable::Moveable() {

	/*worldPos.x = worldPos.y = worldPos.z = 0.0f;
	translation.x = translation.y = translation.z = 0.0f;
	rotation.x = rotation.y = rotation.z = 0.0f;
	localPos.x = localPos.y = localPos.z = 0.0f;
	scale.x = scale.y = scale.z = 1.0f;*/

	worldPos = glm::vec3(0.0);
	localPos = glm::vec3(0.0);

	translation = glm::vec3(0.0);
	rotation = glm::vec3(0.0);
	scale = glm::vec3(1.0);
}

Moveable::Moveable(Moveable* copy) {

	//memcpy(worldPos, copy->worldPos, sizeof(worldPos));
	//memcpy(translation, copy->translation, sizeof(translation));
	//memcpy(rotation, copy->rotation, sizeof(rotation));
	//memcpy(scale, copy->scale, sizeof(scale));
	//localPos.x = localPos.y = localPos.z = 0.0f;

	//	memcpy(worldPos, copy->worldPos, sizeof(worldPos));
	//	memcpy(translation, copy->translation, sizeof(translation));
	//	memcpy(rotation, copy->rotation, sizeof(rotation));
	//	memcpy(scale, copy->scale, sizeof(scale));
	//	localPos[0] = localPos[1] = localPos[2] = 0.0f;

	worldPos = glm::vec3(copy->GetWorldPosVec());
	translation = glm::vec3(copy->GetTranlationVec());
	rotation = glm::vec3(copy->GetRotationVec());
	scale = glm::vec3(copy->GetScaleVec());

	localPos = glm::vec3(0.0);
}

Moveable::~Moveable()
{
}

// ======================================

void Moveable::SetWorldPos(byte* buffer)
{
	worldPos.x = *(float*)buffer;
	worldPos.y = *(float*)(buffer + 4);
	worldPos.z = *(float*)(buffer + 8);
}

// ======================================

void Moveable::SetTranslation(byte* buffer)
{
	translation.x = *(float*)buffer;
	translation.y = *(float*)(buffer + 4);
	translation.z = *(float*)(buffer + 8);
}

void Moveable::SetTranslation(float x, float y, float z)
{
	translation.x = x;
	translation.y = y;
	translation.z = z;
}

// ======================================

void Moveable::SetRotation(byte* buffer)
{
	rotation.x = *(float*)buffer;
	rotation.y = *(float*)(buffer + 4);
	rotation.z = *(float*)(buffer + 8);
}

void Moveable::SetRotation(float* rot)
{
	rotation.x = rot[0];
	rotation.y = rot[1];
	rotation.z = rot[2];
}

void Moveable::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void Moveable::ShiftRotation(float* rot)
{
	rotation.x += rot[0];
	rotation.y += rot[1];
	rotation.z += rot[2];
}

void Moveable::ShiftRotation(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

// ======================================

void Moveable::SetLocalPos(float x, float y, float z)
{
	localPos.x = x;
	localPos.y = y;
	localPos.z = z;
}

void Moveable::SetLocalPos(byte* buffer)
{
	localPos.x = *(float*)buffer;
	localPos.y = *(float*)(buffer + 4);
	localPos.z = *(float*)(buffer + 8);
}

// ======================================

void Moveable::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Moveable::SetScale(byte* buffer)
{
	scale.x = *(float*)buffer;
	scale.y = *(float*)(buffer + 4);
	scale.z = *(float*)(buffer + 8);
}

//#include "Moveable.h"
//
//
//
//Moveable::Moveable() {
//
//	worldPos[0] = worldPos[1] = worldPos[2] = 0.0f;
//	translation[0] = translation[1] = translation[2] = 0.0f;
//	rotation[0] = rotation[1] = rotation[2] = 0.0f;
//	localPos[0] = localPos[1] = localPos[2] = 0.0f;
//	scale[0] = scale[1] = scale[2] = 1.0f;
//}
//
//Moveable::Moveable(Moveable* copy) {
//
//	memcpy(worldPos, copy->worldPos, sizeof(worldPos));
//	memcpy(translation, copy->translation, sizeof(translation));
//	memcpy(rotation, copy->rotation, sizeof(rotation));
//	memcpy(scale, copy->scale, sizeof(scale));
//	localPos[0] = localPos[1] = localPos[2] = 0.0f;
//}
//
//Moveable::~Moveable()
//{
//}
//
//// ======================================
//
//void Moveable::SetWorldPos(byte* buffer)
//{
//	worldPos[0] = *(float*)buffer;
//	worldPos[1] = *(float*)(buffer + 4);
//	worldPos[2] = *(float*)(buffer + 8);
//}
//
//// ======================================
//
//void Moveable::SetTranslation(byte* buffer)
//{
//	translation[0] = *(float*)buffer;
//	translation[1] = *(float*)(buffer + 4);
//	translation[2] = *(float*)(buffer + 8);
//}
//
//void Moveable::SetTranslation(float x, float y, float z)
//{
//	translation[0] = x;
//	translation[1] = y;
//	translation[2] = z;
//}
//
//// ======================================
//
//void Moveable::SetRotation(byte* buffer)
//{
//	rotation[0] = *(float*)buffer;
//	rotation[1] = *(float*)(buffer + 4);
//	rotation[2] = *(float*)(buffer + 8);
//}
//
//void Moveable::SetRotation(float* rot)
//{
//	rotation[0] = rot[0];
//	rotation[1] = rot[1];
//	rotation[2] = rot[2];
//}
//
//void Moveable::SetRotation(float x, float y, float z)
//{
//	rotation[0] = x;
//	rotation[1] = y;
//	rotation[2] = z;
//}
//
//void Moveable::ShiftRotation(float* rot)
//{
//	rotation[0] += rot[0];
//	rotation[1] += rot[1];
//	rotation[2] += rot[2];
//}
//
//void Moveable::ShiftRotation(float x, float y, float z)
//{
//	rotation[0] += x;
//	rotation[1] += y;
//	rotation[2] += z;
//}
//
//// ======================================
//
//void Moveable::SetLocalPos(float x, float y, float z)
//{
//	localPos[0] = x;
//	localPos[1] = y;
//	localPos[2] = z;
//}
//
//void Moveable::SetLocalPos(byte* buffer)
//{
//	localPos[0] = *(float*)buffer;
//	localPos[1] = *(float*)(buffer + 4);
//	localPos[2] = *(float*)(buffer + 8);
//}
//
//// ======================================
//
//void Moveable::SetScale(float x, float y, float z)
//{
//	scale[0] = x;
//	scale[1] = y;
//	scale[2] = z;
//}
//
//void Moveable::SetScale(byte* buffer)
//{
//	scale[0] = *(float*)buffer;
//	scale[1] = *(float*)(buffer + 4);
//	scale[2] = *(float*)(buffer + 8);
//}