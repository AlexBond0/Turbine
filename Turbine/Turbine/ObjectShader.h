#pragma once

#include "Shader.h"

class ObjectShader
	: public Shader
{
public:
	ObjectShader(const wchar_t* vertshader, const wchar_t* fragshader);
	~ObjectShader();
};

