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

	// will be implemented better later
	void IsHighlighted(bool highlighted);

protected:

	float glossiness, speclevel;

private:

	color4 _ambient, _diffuse, _specular;
	color4	_highlight;
	bool	_isHighlighted;
};

inline const color4 Material::GetAmbient() {

	if (_isHighlighted)
		return _highlight;
	else
		return _ambient;
}

inline const color4 Material::GetDiffuse() {

	if (_isHighlighted)
		return _highlight;
	else
		return _diffuse;
}

inline const color4 Material::GetSpecular() {

	return _specular;
}

inline const float Material::GetGlossiness() {

	return glossiness;
}

inline const float Material::GetSpecLevel() {

	return speclevel;
}