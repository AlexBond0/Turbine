#include "LuaEditorUI.h"



LuaEditorUI::LuaEditorUI() {

	// creating the language and editor
	auto language = TextEditor::LanguageDefinition::Lua();

	// example of tooltip added into the editor
	TextEditor::Identifier msg;
	msg.mDeclaration = "Turbine Callback\nCalled when the Entity receives a message";
	language.mIdentifiers["OnMessage"] = msg;

	editor.SetLanguageDefinition(language);
	editor.SetShowWhitespaces(false);

	// loading a file
	fileToEdit = "Scripts/Test3.lua";
	{
		std::ifstream t(fileToEdit);
		if (t.good())
		{
			std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			editor.SetText(str);

			_Compile(true);
		}
	}

	LoadTurbineTypes();
}

LuaEditorUI::~LuaEditorUI() {


}

void LuaEditorUI::Render() {

	ImGui::Begin("Lua Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);

	auto cpos = editor.GetCursorPosition();

	_RenderMenuBar();

	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		editor.IsOverwrite() ? "Ovr" : "Ins",
		editor.CanUndo() ? "*" : " ",
		editor.GetLanguageDefinition().mName.c_str(), fileToEdit
	);

	// compile status
	if (_markers.size() != 0) {

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Compile Error");

		if (ImGui::IsItemHovered())
			ImGui::Text(_topErrMsg.c_str());
	}
	else
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Compile OK");

	ImGui::Separator();

	editor.Render("TextEditor");
	ImGui::End();
}

void LuaEditorUI::_RenderMenuBar() {

	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Save")) 
				_AttemptSave();
			
			if (ImGui::MenuItem("Compile"))
				_Compile(false);

			// if (ImGui::MenuItem("Quit", "Alt-F4"))
			//	break;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) {

			bool ro = editor.IsReadOnly();

			if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
				editor.SetReadOnly(ro);

			ImGui::Separator();

			if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
				editor.Undo();
			if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
				editor.Redo();

			ImGui::Separator();

			if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
				editor.Copy();
			if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
				editor.Cut();
			if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
				editor.Delete();
			if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
				editor.Paste();

			ImGui::Separator();

			if (ImGui::MenuItem("Select all", nullptr, nullptr))
				editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View")) {

			if (ImGui::MenuItem("Dark palette"))
				editor.SetPalette(TextEditor::GetDarkPalette());
			if (ImGui::MenuItem("Light palette"))
				editor.SetPalette(TextEditor::GetLightPalette());
			if (ImGui::MenuItem("Retro blue palette"))
				editor.SetPalette(TextEditor::GetRetroBluePalette());
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

bool LuaEditorUI::_Compile(bool fromFile) {

	// create a lua state
	sol::state lua;

	// open libs
	lua.open_libraries(sol::lib::math);
	lua.open_libraries(sol::lib::base);
	lua.open_libraries(sol::lib::string);

	sol::protected_function_result result;
	
	// run the script contained in the file itself
	if (fromFile) {

		result = lua.script_file(
			fileToEdit,
			[](lua_State*, sol::protected_function_result pfr) {

			return pfr;
		});
	}

	// run the script currently in the editor
	else {

		result = lua.script(
			editor.GetText(),
			[](lua_State*, sol::protected_function_result pfr) {

			return pfr;
		});
	}

	_markers.clear();

	// compiled incorrectly
	if (!result.valid()) {

		sol::error err = result;
		std::string what = err.what();

		// get tokens
		std::vector<std::string> tokens = SplitToTokens(what, ':');

		// error with linenumber
		if (tokens.size() == 3) {

			int errorLoc = std::stoi(tokens[1]);

			if (errorLoc > editor.GetTotalLines())
				errorLoc = editor.GetTotalLines();

			_markers.insert(std::make_pair(
				errorLoc,
				tokens[2].c_str()
			));
		}

		// other error type
		else {

			_markers.insert(std::make_pair<int, std::string>(0, what.c_str()));
		}

		_topErrMsg = what;
	}

	editor.SetErrorMarkers(_markers);

	return (_markers.size() == 0);
}

// check if file compiles then save
void LuaEditorUI::_AttemptSave() {

	// make sure everything compiles first
	if (_Compile(false)) {

		auto textToSave = editor.GetText();

		// save file
		std::ofstream myfile(fileToEdit);
		if (myfile.is_open()) {

			myfile << textToSave;
			myfile.close();
		}
	}
}

void LuaEditorUI::LoadTurbineTypes() {

	auto language = TextEditor::LanguageDefinition::Lua();

	// normal ones
	for (auto const& type : TurbineUsertypeDefiner::descriptions) {

		TextEditor::Identifier msg;
		msg.mDeclaration = type.second;
		language.mIdentifiers[type.first] = msg;
	}

	// dictionary ones
	editor.dictionary = TurbineUsertypeDefiner::dictionary;

	editor.SetLanguageDefinition(language);
}