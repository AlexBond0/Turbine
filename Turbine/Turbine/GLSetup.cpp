#include "GLSetup.h"
#include <stdio.h>
#include <io.h>
#include "Utils.h"

BOOL SetupPixelFormat(HDC hdc);
int LoadProgram(int type, const wchar_t* filename);
bool LinkProgram(int program, int shader, int fragment);

HDC InitGL(HWND parent) {

	OutputDebugStringA("\n\nInitialising Win32 OpenGL...");

	HDC hdcNew=::GetDC(parent);

	if (!SetupPixelFormat(hdcNew))
	return 0;

	HGLRC hrc=wglCreateContext(hdcNew);
	wglMakeCurrent(hdcNew, hrc);

	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);

	return hdcNew;
}

BOOL SetupPixelFormat(HDC hdc)
{
  int pixelformat;
  PIXELFORMATDESCRIPTOR pfd = 
	{
    sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
    1,                              // version number
    PFD_DRAW_TO_WINDOW |            // support window
    PFD_SUPPORT_OPENGL |            // support OpenGL
    PFD_DOUBLEBUFFER,               // double buffered
    PFD_TYPE_RGBA,                  // RGBA type
    32,                             // 32-bit color depth
    0, 0, 0, 0, 0, 0,               // color bits ignored
    0,                              // no alpha buffer
    0,                              // shift bit ignored
    0,                              // no accumulation buffer
    0, 0, 0, 0,                     // accum bits ignored
    32,                             // 32-bit z-buffer
    32,                             // 32-bit stencil buffer  - !no stencil buffer
    0,                              // no auxiliary buffer
    PFD_MAIN_PLANE,                 // main layer
    0,                              // reserved
    0, 0, 0                         // layer masks ignored
  };

	if (!(pixelformat=ChoosePixelFormat(hdc, &pfd))) {

		MessageBox(NULL, "ChoosePixelFormat failed", "GLSetup", MB_OK);
		OutputDebugStringA("\nChoosePixelFormat FAILED");
		return FALSE;
	}
	else {

		OutputDebugStringA("\nChoosePixelFormat OK");
	}

	if (!SetPixelFormat(hdc, pixelformat, &pfd)) {

		MessageBox(NULL, "SetPixelFormat failed", "GLSetup", MB_OK);
		OutputDebugStringA("\SetPixelFormat FAILED");
		return FALSE;
	}
	else {

		OutputDebugStringA("\SetPixelFormat OK");
	}

  return TRUE;
}

int LoadShaders(const wchar_t* vertshader, const wchar_t* fragshader) {

	OutputDebugStringA("\n\nLOADING SHADERS...\n");

	int vhandle=LoadProgram(GL_VERTEX_SHADER, vertshader); 
	int fhandle=LoadProgram(GL_FRAGMENT_SHADER, fragshader);

	if (vhandle!=0 && fhandle!=0) {

		int phandle=glCreateProgram();

		if (LinkProgram(phandle, vhandle, fhandle)) {

			return phandle;
		}
		else {

			glDeleteProgram(phandle);
			glDeleteShader(fhandle);
			glDeleteShader(vhandle);
		}
	}
	return 0;
}

int LoadProgram(int type, const wchar_t* filename) {

	// get shader type as string
	std::string shaderType;
	switch (type) {

		case GL_VERTEX_SHADER: {	shaderType = "VERTEX";	break; }
		case GL_FRAGMENT_SHADER: {	shaderType = "FRAGMENT";	break; }
		default: {	shaderType = std::to_string(type);	break; }
	}

	int shader=glCreateShader(type);
    
	if (shader!=0) {

		// gen the filepath
		FILE* file=NULL;
		_wfopen_s(&file, filename, L"rb");

		if (file) {

			// load the file into the char* data
			int len=_filelength(_fileno(file));
			char* data=(char*) malloc(len);
			int read=0;
			int pos=0;
			do
			{
			read=fread(data+pos, 1, len-pos, file);
			if (read>0)
				pos+=read;
			}
			while (read>0 && pos!=len);
			fclose(file);

			// generate the shader
			glShaderSource(shader, 1, &data, &len);
			glCompileShader(shader);
			free(data);
      
			int status;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

			if(status==GL_FALSE) 
			{
				// get the shader info log
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
				char* error=(char*) malloc(len+1);
				glGetShaderInfoLog(shader, len, &status, error);
				error[len]='\0';
				OutputDebugStringA(("\n" + shaderType + " SHADER COMPILE ERROR\n").c_str());
				OutputDebugStringA(error);
				free(error);

				glDeleteShader(shader);
				shader=0;
			}
			else {
				OutputDebugStringA(("\n" + shaderType + " SHADER COMPILE OK").c_str());
			}
		}
		else {

			OutputDebugStringA(("\n" + shaderType + " SHADER MISSING!\n").c_str());
			glDeleteShader(shader);
			shader=0;
		}
	}
	return shader;
}

bool LinkProgram(int program, int shader, int fragment) {

	glAttachShader(program, shader);
	glAttachShader(program, fragment);

	glLinkProgram(program);
      
	int status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status==GL_FALSE)
	{
		int length;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char* error=(char*) malloc(length+1);
		glGetProgramInfoLog(program, length, &status, error);
		error[length]='\0';
		OutputDebugStringA("\nPROGRAM BUILD ERROR\n");
		OutputDebugStringA(error);
		free(error);
		return false;
	}
	else {

		OutputDebugStringA("\nPROGRAM BUILD OK\n");

		glDeleteShader(shader);
		glDeleteShader(fragment);
	}

	return true;
}

// helper function to log shader linkage
void ShaderLinkLog(int handle, std::string attribName) {

	std::string message;

	if (handle == -1)
		message = "SHADER LINK ERROR : " + attribName + " linked unsuccessfully\n";

	else if (handle > 100)
		message = "SHADER LINK WARN : " + attribName + " may have been compiled away\n";

	else
		message = "SHADER LINK OK : " + attribName + " linked successfully\n";

	OutputDebugStringA(message.c_str());
}

// helper function to link a shader attribute and log the result
void LinkAttribHandle(int& attribHandle, std::string attribName, int progHandle) {

	attribHandle = glGetAttribLocation(progHandle, attribName.c_str());
	ShaderLinkLog(attribHandle, attribName.c_str());
}

// helper function to link a shader uniform and log the result
void LinkUniformHandle(int& attribHandle, std::string attribName, int progHandle) {

	attribHandle = glGetUniformLocation(progHandle, attribName.c_str());
	ShaderLinkLog(attribHandle, attribName.c_str());
}