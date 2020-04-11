#pragma once

#include "ImGui//imgui.h"
#include "Light.h"
#include <string>

#include "DebugUIContainer.h"

class MoveableUI 
	: public DebugUIContainer
{
public:
	MoveableUI();
	~MoveableUI();

	void Render();

	Light* light;
	double timePassed;
};

class FramesUI
{
public:
	FramesUI();
	~FramesUI();

	double timePassed;
	void Render();
};