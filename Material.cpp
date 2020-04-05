#include "Material.h"



Material::Material() {

	ambient[0] = ambient[1] = ambient[2] = 0.2f;
	ambient[3] = 1.0f;

	diffuse[0] = diffuse[1] = diffuse[2] = 0.8f;
	diffuse[3] = 1.0f;

	specular[0] = specular[1] = specular[2] = 0;
	specular[3] = 1.0f;

	speclevel = 5;
	glossiness = 25;
}

Material::Material(Material* copy) {

	memcpy(ambient, copy->ambient, sizeof(ambient));
	memcpy(diffuse, copy->diffuse, sizeof(diffuse));
	memcpy(specular, copy->specular, sizeof(specular));

	speclevel = copy->speclevel;
	glossiness = copy->glossiness;
}

Material::~Material()
{
}

void Material::SetMaterial(byte* buffer)
{
	int i;
	int offset = 0;
	for (i = 0; i < 4; i++, offset += 4)
	{
		ambient[i] = *(float*)(buffer + offset);
		diffuse[i] = *(float*)(buffer + offset + 16);
		specular[i] = *(float*)(buffer + offset + 32);
	}

	WORD tmp = *(WORD*)(buffer + 48);
	glossiness = (float)tmp;
	if (glossiness > 100)
		glossiness = 100;

	tmp = *(WORD*)(buffer + 50);
	speclevel = (float)tmp;

	if (speclevel > 100)
		speclevel = 100;

	float spec = speclevel / 100.0f;
	for (int i = 0; i < 4; i++)
		specular[i] *= spec;
}

// ======================================

void Material::SetAmbient(float r, float g, float b, float a) {

	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;
}

void Material::SetAmbient(byte* buffer) {

	ambient[0] = *(float*)buffer;
	ambient[1] = *(float*)(buffer + 4);
	ambient[2] = *(float*)(buffer + 8);
	ambient[3] = *(float*)(buffer + 12);
}

// ======================================

void Material::SetDiffuse(float r, float g, float b, float a) {

	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
}

void Material::SetDiffuse(byte* buffer) {

	diffuse[0] = *(float*)buffer;
	diffuse[1] = *(float*)(buffer + 4);
	diffuse[2] = *(float*)(buffer + 8);
	diffuse[3] = *(float*)(buffer + 12);
}

// ======================================

void Material::SetSpecular(float r, float g, float b, float a) {

	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;
}

void Material::SetSpecular(byte* buffer) {

	specular[0] = *(float*)buffer;
	specular[1] = *(float*)(buffer + 4);
	specular[2] = *(float*)(buffer + 8);
	specular[3] = *(float*)(buffer + 12);
}

// ======================================

void Material::SetGlossiness(float g) {

	glossiness = g;
}

// ======================================

void Material::SetSpecLevel(float s) {

	speclevel = s;
}