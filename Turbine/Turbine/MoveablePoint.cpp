#include "MoveablePoint.h"

MoveablePoint::MoveablePoint() {

	_worldPos = glm::vec3(0.0);
	_localPos = glm::vec3(0.0);

	_translation = glm::vec3(0.0);
	_scale = glm::vec3(1.0);

	_up = glm::vec3(0.0, 1.0, 0.0);
}

MoveablePoint::MoveablePoint(MoveablePoint* copy) {

	_worldPos = copy->_worldPos;
	_localPos = copy->_localPos; // or glm::vec3(0.0);

	_translation = copy->_translation;
	_scale = copy->_scale;

	_up = copy->_up;
}

// ======================================

// Set the position of the element in world space
void MoveablePoint::SetWorldPos(byte* buffer)
{
	_worldPos.x = *(float*)buffer;
	_worldPos.y = *(float*)(buffer + 4);
	_worldPos.z = *(float*)(buffer + 8);
}

// Set the position of the element in world space
void MoveablePoint::SetWorldPos(glm::vec3 pos)
{
	_worldPos = pos;
}

glm::vec3* MoveablePoint::GetWorldPosVec() {

	return &_worldPos;
}


// ======================================

// Set the _translation of the element
void MoveablePoint::SetTranslation(byte* buffer)
{
	_translation.x = *(float*)buffer;
	_translation.y = *(float*)(buffer + 4);
	_translation.z = *(float*)(buffer + 8);
}

// Set the _translation of the element
void MoveablePoint::SetTranslation(float x, float y, float z)
{
	_translation.x = x;
	_translation.y = y;
	_translation.z = z;
}

// Set the _translation of the element
void MoveablePoint::SetTranslation(glm::vec3 pos) {

	_translation = pos;
}

glm::vec3* MoveablePoint::GetTranlationVec() {

	return &_translation;
}

// ======================================

void MoveablePoint::SetUp(glm::vec3 direction) {

	_up = direction;
}

glm::vec3* MoveablePoint::GetUpVec() {

	return &_up;
}

// ======================================

// Set the local position of the element relative to world space
void MoveablePoint::SetLocalPos(float x, float y, float z)
{
	_localPos.x = x;
	_localPos.y = y;
	_localPos.z = z;
}

// Set the local position of the element relative to world space
void MoveablePoint::SetLocalPos(byte* buffer)
{
	_localPos.x = *(float*)buffer;
	_localPos.y = *(float*)(buffer + 4);
	_localPos.z = *(float*)(buffer + 8);
}

// Set the local position of the element relative to world space
void MoveablePoint::SetLocalPos(glm::vec3 pos) {

	_localPos = pos;
}

glm::vec3* MoveablePoint::GetLocalPosVec() {

	return &_localPos;
}

// ======================================

// Set the _scale of the element
void MoveablePoint::SetScale(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
}

// Set the _scale of the element
void MoveablePoint::SetScale(float newScale)
{
	_scale.x = newScale;
	_scale.y = newScale;
	_scale.z = newScale;
}

// Set the _scale of the element
void MoveablePoint::SetScale(byte* buffer)
{
	_scale.x = *(float*)buffer;
	_scale.y = *(float*)(buffer + 4);
	_scale.z = *(float*)(buffer + 8);
}

// Set the _scale of the element
void MoveablePoint::SetScale(glm::vec3 newScale) {

	_scale = newScale;
}


glm::vec3* MoveablePoint::GetScaleVec() {

	return &_scale;
}