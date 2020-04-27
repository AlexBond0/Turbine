#pragma once

#include "ImGui//imgui.h"
#include "DebugUIContainer.h"

#include "Object3D.h"
#include <string>

class Object3DUI 
	: public DebugUIContainer
{
public:
	Object3DUI();
	~Object3DUI();

	void Render();

	Object3D* object;
};

class FramesUI
{
public:
	FramesUI();
	~FramesUI();

	double timePassed;
	void Render();
};