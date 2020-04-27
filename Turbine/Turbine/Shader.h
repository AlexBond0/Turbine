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

	// =====================================================

	void SetupObjectShader();

protected:

	void _StartLinking(bool verbose);
	void _LinkUniformHandle(const std::string &name);
	void _EndLinking();

private:

	unsigned int _GetHandle(const std::string &name);

	bool _handleLinkError = false; // has one of the uniform links failed
	bool _verbose = false; // show all messages
};

// ================================================================================

inline void Shader::SetBool(const std::string &name, bool value) {

	glUniform1i(_GetHandle(name), (int)value);
}

inline void Shader::SetInt(const std::string &name, int value) {

	glUniform1i(_GetHandle(name), value);
}

inline void Shader::SetFloat(const std::string &name, float value) {

	glUniform1f(_GetHandle(name), value);
}

inline void Shader::SetColor(const std::string &name, float* value) {

	glUniform4fv(_GetHandle(name), 1, value);
}

inline void Shader::SetVector(const std::string &name, glm::vec3& value) {

	glUniform3fv(_GetHandle(name), 1, glm::value_ptr(value));
}

inline void Shader::SetMatrix(const std::string &name, glm::mat4& value) {

	glUniformMatrix4fv(_GetHandle(name), 1, false, glm::value_ptr(value));
}

inline unsigned int Shader::_GetHandle(const std::string &name) {

	// handle provided doesnt exist, needs defining
	assert(handles.find(name) != handles.end());

	return handles[name];
}

inline void Shader::_StartLinking(bool vebose = false) {

	_verbose = vebose;
	_handleLinkError = false;
}

// helper function to link a shader uniform and log the result
inline void Shader::_LinkUniformHandle(const std::string &name) {

	// get handle
	unsigned int attribHandle = glGetUniformLocation(ID, name.c_str());

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

inline void Shader::_EndLinking() {

	std::string message;

	if (_handleLinkError)
		message = "SHADER LINKING UNSUCCESSFUL!\n";

	else 
		message = "SHADER LINKING SUCCESSFUL\n";

	OutputDebugStringA(message.c_str());
}
