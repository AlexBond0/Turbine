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
	void _AttemptSave();

	TextEditor::ErrorMarkers _markers;
	bool _Compile(bool fromFile);

	std::string _topErrMsg;
};

