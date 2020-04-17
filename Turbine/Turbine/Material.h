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
	color4* GetAmbient();

	void SetDiffuse(float r, float g, float b, float a);
	void SetDiffuse(color4 color);
	void SetDiffuse(byte* buffer);
	color4* GetDiffuse();

	void SetSpecular(float r, float g, float b, float a);
	void SetSpecular(color4 color);
	void SetSpecular(byte* buffer);
	color4* GetSpecular();

	void SetGlossiness(float g);
	float* GetGlossiness();

	void SetSpecLevel(float s);
	float* GetSpecLevel();

	bool isHighlighted; // will be implemented better later

protected:

	float glossiness, speclevel;

private:

	color4 _ambient, _diffuse, _specular;
	color4	_highlight;
};

inline color4* Material::GetAmbient() {

	if (isHighlighted)
		return &_highlight;
	else
		return &_ambient;
}

inline color4* Material::GetDiffuse() {

	if (isHighlighted)
		return &_highlight;
	else
		return &_diffuse;
}

inline color4* Material::GetSpecular() {

	return &_specular;
}

inline float* Material::GetGlossiness() {

	return &glossiness;
}

inline float* Material::GetSpecLevel() {

	return &speclevel;
}