#include "Shader.h"



Shader::Shader(const wchar_t* vertshader, const wchar_t* fragshader) {

	ID = LoadShaders(L"shader.vert", L"shader.frag");
}


Shader::~Shader() {

	glDeleteProgram(ID);
}

// use/activate the shader
void Shader::Use() {

	glUseProgram(ID);
}


void Shader::SetBool(const std::string &name, bool value) const {

	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const {

	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {

	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetColor(const std::string &name, float* value) const {

	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::SetVector(const std::string &name, glm::vec3& value) const {

	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetMatrix(const std::string &name, glm::mat4& value) const {

	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, glm::value_ptr(value));
}