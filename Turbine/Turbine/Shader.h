#pragma once

#include "GLSetup.h"
#include "Matrix.h"
#include <string>
#include <map>

class Shader
{
public:
	// constructor reads and builds the shader
	Shader(const wchar_t* vertshader, const wchar_t* fragshader);
	~Shader();

	// the program ID
	unsigned int ID;

	std::map<std::string, unsigned int> handles;

	// use/activate the shader
	void Use();
	// utility uniform functions
	void SetBool(const std::string &name, bool value);
	void SetInt(const std::string &name, int value);
	void SetFloat(const std::string &name, float value);

	void SetColor(const std::string &name, float* value);

	void SetVector(const std::string &name, glm::vec3& value);
	void SetMatrix(const std::string &name, glm::mat4& value);

protected:

	void _LinkUniformHandle(const std::string &name);

private:

	unsigned int _GetHandle(const std::string &name);
};

