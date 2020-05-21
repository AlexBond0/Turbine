#pragma once

#include "DebugUIContainer.h"
#include "ImGui/imgui_ext_TextEditor.h"
#include <fstream>

#include "Sol2//sol.hpp"
#include "Utils.h"

class LuaEditorUI
	: public DebugUIContainer
{

public:

	LuaEditorUI();
	~LuaEditorUI();

	void Render();

	TextEditor editor;
	char* fileToEdit;

private:


	void _RenderMenuBar();

	TextEditor::ErrorMarkers _markers;
	void _Compile(bool fromFile);


};

