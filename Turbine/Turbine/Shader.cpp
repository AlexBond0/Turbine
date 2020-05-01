#include "Shader.h"



Shader::Shader(std::string shaderName, const wchar_t* vertshader, const wchar_t* fragshader) {

	name = shaderName;
	std::string msg = "\nCreating shader : " + name + "...";
	OutputDebugStringA(msg.c_str());

	ID = LoadShaders(vertshader, fragshader);
}

Shader::~Shader() {

	glDeleteProgram(ID);
}

// use/activate the shader
void Shader::Use() {

	glUseProgram(ID);
}

// ================================================================================

void Shader::SetBool(const std::string &name, bool value) {

	glUniform1i(_GetHandle(name), (int)value);
}

void Shader::SetInt(const std::string &name, int value) {

	glUniform1i(_GetHandle(name), value);
}

void Shader::SetFloat(const std::string &name, float value) {

	glUniform1f(_GetHandle(name), value);
}

void Shader::SetColor(const std::string &name, float* value) {

	glUniform4fv(_GetHandle(name), 1, value);
}

void Shader::SetVector(const std::string &name, glm::vec3& value) {

	glUniform3fv(_GetHandle(name), 1, glm::value_ptr(value));
}

void Shader::SetMatrix(const std::string &name, glm::mat4& value) {

	glUniformMatrix4fv(_GetHandle(name), 1, false, glm::value_ptr(value));
}

unsigned int Shader::_GetHandle(const std::string &name) {

	// handle provided doesnt exist, needs defining
	if (handles.find(name) == handles.end())
		_LinkUniformHandle(name);

	return handles[name];
}

void Shader::_StartLinking(bool vebose = false) {

	_verbose = vebose;
	_handleLinkError = false;
}

// helper function to link a shader uniform and log the result
void Shader::_LinkUniformHandle(const std::string &name) {

	// get handle
	unsigned int attribHandle = glGetUniformLocation(ID, name.c_str());

	if (!_hasLinkedBefore) {

		std::string msg = "\nLinking shader : " + name + "...\n";
		OutputDebugStringA(msg.c_str());
		_hasLinkedBefore = true;
	}

	if (_verbose) {

		// correct handle found?
		std::string message;
		if (attribHandle == -1) {

			message = "SHADER LINK ERROR : " + name + " linked unsuccessfully\n";
			_handleLinkError = true;
		}

		else if (attribHandle > 100)
			message = "SHADER LINK WARN : " + name + " may have been compiled away\n";

		else
			message = "SHADER LINK OK : " + name + " linked successfully\n";

		OutputDebugStringA(message.c_str());
	}
	else {

		if (attribHandle == -1)
			_handleLinkError = true;
	}

	handles[name] = attribHandle;
}

void Shader::_EndLinking() {

	std::string message;

	if (_handleLinkError)
		message = "SHADER LINKING UNSUCCESSFUL!\n";

	else
		message = "SHADER LINKING SUCCESSFUL\n";

	OutputDebugStringA(message.c_str());
}
