#include "Moveable.h"



Moveable::Moveable() {

	worldPos = glm::vec3(0.0);
	localPos = glm::vec3(0.0);

	translation = glm::vec3(0.0);
	rotation = glm::vec3(0.0);
	scale = glm::vec3(1.0);
}

Moveable::Moveable(Moveable* copy) {

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

// Set the position of the element in world space
void Moveable::SetWorldPos(byte* buffer)
{
	worldPos.x = *(float*)buffer;
	worldPos.y = *(float*)(buffer + 4);
	worldPos.z = *(float*)(buffer + 8);
}

// ======================================

// Set the translation of the element
void Moveable::SetTranslation(byte* buffer)
{
	translation.x = *(float*)buffer;
	translation.y = *(float*)(buffer + 4);
	translation.z = *(float*)(buffer + 8);
}

// Set the translation of the element
void Moveable::SetTranslation(float x, float y, float z)
{
	translation.x = x;
	translation.y = y;
	translation.z = z;
}

// ======================================

// Set the rotation of the element
void Moveable::SetRotation(byte* buffer)
{
	rotation.x = *(float*)buffer;
	rotation.y = *(float*)(buffer + 4);
	rotation.z = *(float*)(buffer + 8);
}

// Set the rotation of the element
void Moveable::SetRotation(float* rot)
{
	rotation.x = rot[0];
	rotation.y = rot[1];
	rotation.z = rot[2];
}

// Set the rotation of the element
void Moveable::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

// Rotate the current rotation of the element
void Moveable::ShiftRotation(float* rot)
{
	rotation.x += rot[0];
	rotation.y += rot[1];
	rotation.z += rot[2];
}

// Rotate the current rotation of the element
void Moveable::ShiftRotation(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

// ======================================

// Set the local position of the element relative to world space
void Moveable::SetLocalPos(float x, float y, float z)
{
	localPos.x = x;
	localPos.y = y;
	localPos.z = z;
}

// Set the local position of the element relative to world space
void Moveable::SetLocalPos(byte* buffer)
{
	localPos.x = *(float*)buffer;
	localPos.y = *(float*)(buffer + 4);
	localPos.z = *(float*)(buffer + 8);
}

// ======================================

// Set the scale of the element
void Moveable::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

// Set the scale of the element
void Moveable::SetScale(byte* buffer)
{
	scale.x = *(float*)buffer;
	scale.y = *(float*)(buffer + 4);
	scale.z = *(float*)(buffer + 8);
}