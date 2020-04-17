#pragma once

#include "GLSetup.h"
#include "Matrix.h"
#include <string>

class Shader
{
public:
	// constructor reads and builds the shader
	Shader(const wchar_t* vertshader, const wchar_t* fragshader);
	~Shader();

	// the program ID
	unsigned int ID;

	// use/activate the shader
	void Use();
	// utility uniform functions
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;

	void SetColor(const std::string &name, float* value) const;

	void SetVector(const std::string &name, glm::vec3& value) const;
	void SetMatrix(const std::string &name, glm::mat4& value) const;
};

