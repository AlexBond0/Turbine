#include "Shader.h"



Shader::Shader(const wchar_t* vertshader, const wchar_t* fragshader) {

	ID = LoadShaders(vertshader, fragshader);
}

Shader::~Shader() {

	glDeleteProgram(ID);
}

// use/activate the shader
void Shader::Use() {

	glUseProgram(ID);
}

void Shader::SetupObjectShader() {

	_StartLinking();

	// Light
	_LinkUniformHandle("u_l_direction");
	_LinkUniformHandle("u_l_halfplane");
	_LinkUniformHandle("u_l_ambient");
	_LinkUniformHandle("u_l_diffuse");
	_LinkUniformHandle("u_l_specular");

	// Material
	_LinkUniformHandle("u_m_ambient");
	_LinkUniformHandle("u_m_diffuse");
	_LinkUniformHandle("u_m_specular");
	_LinkUniformHandle("u_m_shininess");

	// texture
	_LinkUniformHandle("u_texture");

	// Matrices
	_LinkUniformHandle("u_normal_matrix");
	_LinkUniformHandle("u_model_matrix");
	_LinkUniformHandle("u_view_matrix");
	_LinkUniformHandle("u_projection_matrix");

	// Flags
	_LinkUniformHandle("u_usesLight");
	_LinkUniformHandle("u_usesTexture");

	// instancing
	_LinkUniformHandle("u_instancing");

	// billboarding
	_LinkUniformHandle("u_billboarding");
	_LinkUniformHandle("a_b_up");
	_LinkUniformHandle("a_b_right");

	_EndLinking();
}