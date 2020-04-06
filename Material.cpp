#include "Material.h"



Material::Material() {


	ambient = color4(1.0, 1.0, 1.0, 1.0);
	diffuse = color4(0.8, 0.8, 0.8, 1.0);
	specular = color4(0.0, 0.0, 0.0, 1.0);

	speclevel = 5;
	glossiness = 25;
}

Material::Material(Material* copy) {

	/*memcpy(ambient, copy->ambient, sizeof(ambient));
	memcpy(diffuse, copy->diffuse, sizeof(diffuse));
	memcpy(specular, copy->specular, sizeof(specular));*/

	ambient = color4(copy->ambient);
	diffuse = color4(copy->diffuse);
	specular = color4(copy->specular);

	speclevel = copy->speclevel;
	glossiness = copy->glossiness;
}

Material::~Material()
{
}

void Material::SetMaterial(byte* buffer)
{
	ambient = color4(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8),
		*(float*)(buffer + 12)
	);

	diffuse = color4(
		*(float*)(buffer + 16),
		*(float*)(buffer + 20),
		*(float*)(buffer + 24),
		*(float*)(buffer + 28)
	);

	specular = color4(
		*(float*)(buffer + 32),
		*(float*)(buffer + 36),
		*(float*)(buffer + 40),
		*(float*)(buffer + 44)
	);


	WORD tmp = *(WORD*)(buffer + 48);
	glossiness = (float)tmp;
	if (glossiness > 100)
		glossiness = 100;

	tmp = *(WORD*)(buffer + 50);
	speclevel = (float)tmp;

	if (speclevel > 100)
		speclevel = 100;

	float spec = speclevel / 100.0f;
	specular.Scale(spec);
}

// ======================================

void Material::SetAmbient(float r, float g, float b, float a) {

	ambient = color4(r, g, b, a);
}

void Material::SetAmbient(byte* buffer) {

	ambient = color4(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8),
		*(float*)(buffer + 12)
	);
}

// ======================================

void Material::SetDiffuse(float r, float g, float b, float a) {

	diffuse = color4(r, g, b, a);
}

void Material::SetDiffuse(byte* buffer) {

	diffuse = color4(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8),
		*(float*)(buffer + 12)
	);
}

// ======================================

void Material::SetSpecular(float r, float g, float b, float a) {

	specular = color4(r, g, b, a);
}

void Material::SetSpecular(byte* buffer) {

	specular = color4(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8),
		*(float*)(buffer + 12)
	);
}

// ======================================

void Material::SetGlossiness(float g) {

	glossiness = g;
}

// ======================================

void Material::SetSpecLevel(float s) {

	speclevel = s;
}