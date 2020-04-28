#include "Moveable.h"



Moveable::Moveable() {

	worldPos = glm::vec3(0.0);
	localPos = glm::vec3(0.0);

	translation = glm::vec3(0.0);
	orientation = glm::quat(glm::vec3(0.0));
	rotation = orientation;
	scale = glm::vec3(1.0);

	up = glm::vec3(0.0, 1.0, 0.0);
}

Moveable::Moveable(Moveable* copy) {

	worldPos = glm::vec3(copy->GetWorldPosVec());
	translation = glm::vec3(copy->GetTranlationVec());
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

// Set the position of the element in world space
void Moveable::SetWorldPos(glm::vec3 pos)
{
	worldPos = pos;
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

void Moveable::SetUp(glm::vec3 direction) {

	up = direction;
}

glm::vec3 Moveable::GetUp() {

	return up;
}

// ======================================

// Set the rotation of the element
void Moveable::SetOrientation(byte* buffer) {

	orientation = glm::quat(glm::vec3(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8)
	));
}

// Set the rotation of the element
void Moveable::SetOrientation(float* rot) {

	orientation = glm::quat(glm::vec3(
		rot[0],
		rot[1],
		rot[2]
	));
}

// Set the rotation of the element
void Moveable::SetOrientation(float x, float y, float z, bool usingRads) {

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

	orientation = glm::quat(rot);
}

// Set the rotation of the element
void Moveable::SetOrientation(glm::quat rot) {

	orientation = rot;
}

// ======================================

// Set the rotation of the element
void Moveable::SetRotation(float x, float y, float z, bool usingRads) {

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

	up = up * rotation;
}

// Set the rotation of the element
void Moveable::SetRotation(glm::vec3 rot) {

	rotation = glm::quat(rot);

	up = up * rotation;
}

// Set the rotation of the element
void Moveable::SetRotation(glm::quat rot) {

	rotation = rot;

	up = up * rotation;
}

// ======================================

// Rotate the current rotation of the element
void Moveable::ShiftRotation(float* rot) {

	glm::quat quatRot = glm::quat(glm::vec3(
		DEGSTORADS(rot[0]),
		DEGSTORADS(rot[1]),
		DEGSTORADS(rot[2])
	));

	rotation += quatRot;
}

// Rotate the current rotation of the element
void Moveable::ShiftRotation(float x, float y, float z) {

	glm::quat quatRot = glm::quat(glm::vec3(
		DEGSTORADS(x),
		DEGSTORADS(y),
		DEGSTORADS(z)
	));

	rotation = glm::normalize(rotation * quatRot);
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
void Moveable::SetScale(float newScale)
{
	scale.x = newScale;
	scale.y = newScale;
	scale.z = newScale;
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
void Moveable::PointAt(glm::vec3 newVector) {

	newVector = glm::normalize(newVector);
	orientation = _RotationBetweenVectors(up, newVector);
}

// Returns a quaternion such that q*start = dest
glm::quat Moveable::_RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {

		// special case when vectors in opposite directions :
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		// This implementation favors a rotation around the Up axis,
		// since it's often what you want to do.
		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return angleAxis(glm::radians(180.0f), rotationAxis);
	}

	// Implementation from Stan Melax's Game Programming Gems 1 article
	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}