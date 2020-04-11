#pragma once

#include <Windows.h>
#include "Color.h"

// Inherit from this class to include simple material properties
class Material 
{
public:
	Material();
	Material(Material* copy);
	~Material();

	void SetMaterial(byte* buffer);

	void SetAmbient(float r, float g, float b, float a);
	void SetAmbient(color4 color);
	void SetAmbient(byte* buffer);
	const color4 GetAmbient();

	void SetDiffuse(float r, float g, float b, float a);
	void SetDiffuse(color4 color);
	void SetDiffuse(byte* buffer);
	const color4 GetDiffuse();

	void SetSpecular(float r, float g, float b, float a);
	void SetSpecular(color4 color);
	void SetSpecular(byte* buffer);
	const color4 GetSpecular();

	void SetGlossiness(float g);
	const float GetGlossiness();

	void SetSpecLevel(float s);
	const float GetSpecLevel();

protected:

	float glossiness, speclevel;
	color4 ambient, diffuse, specular;
};

inline const color4 Material::GetAmbient() {

	return ambient;
}

inline const color4 Material::GetDiffuse() {

	return diffuse;
}

inline const color4 Material::GetSpecular() {

	return specular;
}

inline const float Material::GetGlossiness() {

	return glossiness;
}

inline const float Material::GetSpecLevel() {

	return speclevel;
}