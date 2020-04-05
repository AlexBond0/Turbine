#pragma once

#include <Windows.h>

class Material 
{
public:
	Material();
	Material(Material* copy);
	~Material();

	void SetMaterial(byte* buffer);

	void SetAmbient(float r, float g, float b, float a);
	void SetAmbient(byte* buffer);
	const float* GetAmbient();

	void SetDiffuse(float r, float g, float b, float a);
	void SetDiffuse(byte* buffer);
	const float* GetDiffuse();

	void SetSpecular(float r, float g, float b, float a);
	void SetSpecular(byte* buffer);
	const float* GetSpecular();

	void SetGlossiness(float g);
	const float GetGlossiness();

	void SetSpecLevel(float s);
	const float GetSpecLevel();

protected:

	float glossiness, speclevel;
	float ambient[4], diffuse[4], specular[4];
};

inline const float* Material::GetAmbient() {

	return ambient;
}

inline const float* Material::GetDiffuse() {

	return diffuse;
}

inline const float* Material::GetSpecular() {

	return specular;
}

inline const float Material::GetGlossiness() {

	return glossiness;
}

inline const float Material::GetSpecLevel() {

	return speclevel;
}