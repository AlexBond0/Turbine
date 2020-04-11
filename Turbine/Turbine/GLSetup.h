#pragma once

#define GLEW_STATIC 

#include <Windows.h>
#include "GL\glew.h"
#include <string>

HDC InitGL(HWND parent);
int LoadShaders(const wchar_t* vertshader, const wchar_t* fragshader);

void ShaderLinkLog(int handle, std::string attribName);
void LinkAttribHandle(int& attribHandle, std::string attribName, int progHandle);
void LinkUniformHandle(int& attribHandle, std::string attribName, int progHandle);
