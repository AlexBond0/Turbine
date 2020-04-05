#include "Moveable.h"



Moveable::Moveable() {

	worldPos[0] = worldPos[1] = worldPos[2] = 0.0f;
	translation[0] = translation[1] = translation[2] = 0.0f;
	rotation[0] = rotation[1] = rotation[2] = 0.0f;
	offset[0] = offset[1] = offset[2] = 0.0f;
	scale[0] = scale[1] = scale[2] = 1.0f;
}

Moveable::Moveable(Moveable* copy) {

	memcpy(worldPos, copy->worldPos, sizeof(worldPos));
	memcpy(translation, copy->translation, sizeof(translation));
	memcpy(rotation, copy->rotation, sizeof(rotation));
	memcpy(scale, copy->scale, sizeof(scale));
	offset[0] = offset[1] = offset[2] = 0.0f;
}

Moveable::~Moveable()
{
}

// ======================================

void Moveable::SetWorldPos(byte* buffer)
{
	worldPos[0] = *(float*)buffer;
	worldPos[1] = *(float*)(buffer + 4);
	worldPos[2] = *(float*)(buffer + 8);
}

// ======================================

void Moveable::SetTranslation(byte* buffer)
{
	translation[0] = *(float*)buffer;
	translation[1] = *(float*)(buffer + 4);
	translation[2] = *(float*)(buffer + 8);
}

void Moveable::SetTranslation(float x, float y, float z)
{
	translation[0] = x;
	translation[1] = y;
	translation[2] = z;
}

// ======================================

void Moveable::SetRotation(byte* buffer)
{
	rotation[0] = *(float*)buffer;
	rotation[1] = *(float*)(buffer + 4);
	rotation[2] = *(float*)(buffer + 8);
}

void Moveable::SetRotation(float* rot)
{
	rotation[0] = rot[0];
	rotation[1] = rot[1];
	rotation[2] = rot[2];
}

void Moveable::SetRotation(float x, float y, float z)
{
	rotation[0] = x;
	rotation[1] = y;
	rotation[2] = z;
}

void Moveable::ShiftRotation(float* rot)
{
	rotation[0] += rot[0];
	rotation[1] += rot[1];
	rotation[2] += rot[2];
}

void Moveable::ShiftRotation(float x, float y, float z)
{
	rotation[0] += x;
	rotation[1] += y;
	rotation[2] += z;
}

// ======================================

void Moveable::SetOffset(float x, float y, float z)
{
	offset[0] = x;
	offset[1] = y;
	offset[2] = z;
}

void Moveable::SetOffset(byte* buffer)
{
	offset[0] = *(float*)buffer;
	offset[1] = *(float*)(buffer + 4);
	offset[2] = *(float*)(buffer + 8);
}

// ======================================

void Moveable::SetScale(float x, float y, float z)
{
	scale[0] = x;
	scale[1] = y;
	scale[2] = z;
}

void Moveable::SetScale(byte* buffer)
{
	scale[0] = *(float*)buffer;
	scale[1] = *(float*)(buffer + 4);
	scale[2] = *(float*)(buffer + 8);
}