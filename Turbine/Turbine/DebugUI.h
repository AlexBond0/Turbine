#pragma once

#include "ImGui//imgui.h"
#include "ImGui//imgui_impl_glfw.h"
#include "ImGui//imgui_impl_opengl3.h"

#include "MoveableUI.h"
#include "CameraUI.h"

#include "DebugUIContainer.h"
#include <vector>

#include <Windows.h>

class DebugUI
{
public:
	DebugUI(const char* glsl_version, GLFWwindow* window);
	~DebugUI();

	void Render();

	GLFWwindow* window;

	void AddComponent(DebugUIContainer* newComponent);

private:
	std::vector<DebugUIContainer*> components;
};

