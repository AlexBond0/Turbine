#pragma once

#include "ImGui/imgui_ext_TextEditor.h"

/// This class only works if the base TextEditor class has had its
/// internal scoping chnaged from private: to protected:
/// also _RenderAutocompleteTooltip needs defining and putting into the Render() function
/// as this class only renders nice when things are called precisely within the code

// extends the TextEditor class to add extra functionality
class TurbineTextEditor :
	public TextEditor {

public:


protected:


	void _RenderAutocompleteTooltip();
	void _CreateAutocompleteTooltip();
};

