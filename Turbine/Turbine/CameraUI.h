#pragma once

#include "ImGui//imgui.h"
#include <string>
#include <list>

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
	double timePassed = 1.0f;

private:

	float _currentfps;
	std::vector<float> _frames;

	const float range = 0.1f;
};

