#pragma once

#include "ImGui//imgui.h"
#include <string>

#include "DebugUIContainer.h"
#include "Camera.h"

class CameraUI
	: public DebugUIContainer
{
public:

	CameraUI();
	~CameraUI();

	void Render();

	Camera* camera;
};

