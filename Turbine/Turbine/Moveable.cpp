#include "Moveable.h"



Moveable::Moveable() {

	worldPos = glm::vec3(0.0);
	localPos = glm::vec3(0.0);

	translation = glm::vec3(0.0);
	// rotation = glm::vec3(0.0);
	orientation = glm::quat(glm::vec3(0.0));
	rotation = orientation;
	scale = glm::vec3(1.0);
}

Moveable::Moveable(Moveable* copy) {

	worldPos = glm::vec3(copy->GetWorldPosVec());
	translation = glm::vec3(copy->GetTranlationVec());
	// rotation = glm::vec3(copy->GetRotationEuler());
	orientation = copy->orientation;
	rotation = copy->rotation;
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

// Set the translation of the element
void Moveable::SetTranslation(glm::vec3 pos) {

	translation = pos;
}

// ======================================

// Set the rotation of the element
void Moveable::SetOrientation(byte* buffer)
{
	//rotation.x = *(float*)buffer;
	//rotation.y = *(float*)(buffer + 4);
	//rotation.z = *(float*)(buffer + 8);

	orientation = glm::quat(glm::vec3(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8)
	));
}

// Set the rotation of the element
void Moveable::SetOrientation(float* rot)
{
	//rotation.x = rot[0];
	//rotation.y = rot[1];
	//rotation.z = rot[2];

	orientation = glm::quat(glm::vec3(
		rot[0],
		rot[1],
		rot[2]
	));
}

// Set the rotation of the element
void Moveable::SetOrientation(float x, float y, float z, bool usingRads)
{
	//rotation.x = x;
	//rotation.y = y;
	//rotation.z = z;

	if (usingRads) {

		orientation = glm::quat(glm::vec3(
			x,
			y,
			z
		));
	}
	else {

		orientation = glm::quat(glm::vec3(
			DEGSTORADS(x),
			DEGSTORADS(y),
			DEGSTORADS(z)
		));
	}
	
}

// Set the rotation of the element
void Moveable::SetOrientation(glm::vec3 rot) {

	// rotation = rot;

	orientation = glm::quat(rot);
}

// ======================================

// Set the rotation of the element
void Moveable::SetRotation(float x, float y, float z, bool usingRads)
{
	//rotation.x = x;
	//rotation.y = y;
	//rotation.z = z;

	if (usingRads) {

		rotation = glm::quat(glm::vec3(
			x,
			y,
			z
		));
	}
	else {

		rotation = glm::quat(glm::vec3(
			DEGSTORADS(x),
			DEGSTORADS(y),
			DEGSTORADS(z)
		));
	}
}

// Set the rotation of the element
void Moveable::SetRotation(glm::vec3 rot) {


	rotation = glm::quat(rot);
}

// ======================================

// Rotate the current rotation of the element
void Moveable::ShiftRotation(float* rot)
{
	//rotation.x += rot[0];
	//rotation.y += rot[1];
	//rotation.z += rot[2];

	glm::quat quatRot = glm::quat(glm::vec3(
		DEGSTORADS(rot[0]),
		DEGSTORADS(rot[1]),
		DEGSTORADS(rot[2])
	));

	rotation += quatRot;
}

// Rotate the current rotation of the element
void Moveable::ShiftRotation(float x, float y, float z)
{
	//rotation.x += x;
	//rotation.y += y;
	//rotation.z += z;

	glm::quat quatRot = glm::quat(glm::vec3(
		DEGSTORADS(x),
		DEGSTORADS(y),
		DEGSTORADS(z)
	));

	rotation += quatRot;
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

// Set the local position of the element relative to world space
void Moveable::SetLocalPos(glm::vec3 pos) {

	localPos = pos;
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

// Set the scale of the element
void Moveable::SetScale(glm::vec3 newScale) {

	scale = newScale;
}

// point towards a vector from a given starting vector direction
void Moveable::PointAt(glm::vec3 objectDirection, glm::vec3 newDirection) {

	/*glm::vec3 normStart = glm::normalize(objectDirection);
	glm::vec3 normEnd = glm::normalize(newDirection);

	glm::mat4 lookat = glm::lookAt(
		translation,
		newDirection,
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	glm::quat toRotation = glm::toQuat(lookat);
	*/

	glm::vec3 direction = glm::normalize(objectDirection);
	SetOrientation(
		RADSTODEGS(direction.x),
		RADSTODEGS(direction.y),
		RADSTODEGS(direction.z)
	);

	// rotation = newDirection;
}