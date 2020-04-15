#pragma once

#include "ImGui//imgui.h"
#include "Object3D.h"
#include <string>

#include "DebugUIContainer.h"

class MoveableUI 
	: public DebugUIContainer
{
public:
	MoveableUI();
	~MoveableUI();

	void Render();

	Object3D* object;
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