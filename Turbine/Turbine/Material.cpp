#include "Material.h"



Material::Material() {


	_ambient = color4(1.0, 1.0, 1.0, 1.0);
	_diffuse = color4(0.8, 0.8, 0.8, 1.0);
	_specular = color4(0.0, 0.0, 0.0, 1.0);

	_highlight = color4(1.0, 0.6, 0.0, 1.0);

	_isHighlighted = false;

	speclevel = 5;
	glossiness = 25;
}

Material::Material(Material* copy) {

	_ambient = color4(copy->_ambient);
	_diffuse = color4(copy->_diffuse);
	_specular = color4(copy->_specular);

	speclevel = copy->speclevel;
	glossiness = copy->glossiness;
}

Material::~Material()
{
}

void Material::SetMaterial(byte* buffer)
{
	_ambient = color4(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8),
		*(float*)(buffer + 12)
	);

	_diffuse = color4(
		*(float*)(buffer + 16),
		*(float*)(buffer + 20),
		*(float*)(buffer + 24),
		*(float*)(buffer + 28)
	);

	_specular = color4(
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
	_specular.Scale(spec);
}

// ======================================

void Material::SetAmbient(float r, float g, float b, float a) {

	_ambient = color4(r, g, b, a);
}

void Material::SetAmbient(color4 color) {

	_ambient = color;
}

void Material::SetAmbient(byte* buffer) {

	_ambient = color4(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8),
		*(float*)(buffer + 12)
	);
}

// ======================================

void Material::SetDiffuse(float r, float g, float b, float a) {

	_diffuse = color4(r, g, b, a);
}

void Material::SetDiffuse(color4 color) {

	_diffuse = color;
}

void Material::SetDiffuse(byte* buffer) {

	_diffuse = color4(
		*(float*)buffer,
		*(float*)(buffer + 4),
		*(float*)(buffer + 8),
		*(float*)(buffer + 12)
	);
}

// ======================================

void Material::SetSpecular(float r, float g, float b, float a) {

	_specular = color4(r, g, b, a);
}

void Material::SetSpecular(color4 color) {

	_specular = color;
}

void Material::SetSpecular(byte* buffer) {

	_specular = color4(
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

// will be implemented better later
void Material::IsHighlighted(bool highlighted) {
	
	_isHighlighted = highlighted;
}