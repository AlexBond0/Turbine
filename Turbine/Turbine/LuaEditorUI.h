#pragma once

#include "DebugUIContainer.h"
#include "TurbineTextEditor.h"
#include <fstream>

#include "TurbineUserTypes.h"
#include "Sol2//sol.hpp"
#include "Utils.h"

class LuaEditorUI
	: public DebugUIContainer
{

public:

	LuaEditorUI();
	~LuaEditorUI();

	void Render();

	void LoadTurbineTypes();

	TurbineTextEditor editor;

	char* fileToEdit;

private:


	void _RenderMenuBar();
	void _AttemptSave();

	TextEditor::ErrorMarkers _markers;
	bool _Compile(bool fromFile);

	std::string _topErrMsg;
};