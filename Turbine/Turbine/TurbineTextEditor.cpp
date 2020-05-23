#include "TurbineTextEditor.h"


void TurbineTextEditor::Alphabetize() {


}


// Render the Autocomplete Tooltip (Called internally from base class)
void TurbineTextEditor::_RenderAutocompleteTooltip() {

	auto lineLength = GetLineMaxColumn(mState.mCursorPosition.mLine);

	// is the crsor at the end of the line
	if (mState.mCursorPosition.mColumn == lineLength && lineLength > 1) {

		int cIndex = mState.mCursorPosition.mColumn;
		int lIndex = mState.mCursorPosition.mLine;

		// get last word
		auto lastChar = GetWordAt(Coordinates(lIndex, cIndex - 1));
		auto lastWord = GetWordAt(Coordinates(lIndex, cIndex - 2));

		if (!lastWord.empty() && !lastChar.empty()) {

			// is word in dictionary
			if (dictionary.find(lastWord) != dictionary.end()) {

				// is the last char an accessor
				if (lastChar.compare(":") == 0 || lastChar.compare(".") == 0) {

					// get dimentions needed for rendering in editor
					ImVec2 screen = ImGui::GetCursorScreenPos();
					auto textSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, -1.0f, mLineBuffer.c_str(), nullptr, nullptr);

					// calculate offset
					auto start = ImVec2(
						screen.x + mTextStart + 10.0f,
						screen.y + (textSize.y * lIndex)
					);

					// caluclate position along screen
					ImVec2 renderPos(
						start.x + TextDistanceToLineStart(Coordinates(lIndex, GetLineMaxColumn(lIndex))),
						start.y
					);

					// only render within screen
					float windowStart = ImGui::GetWindowPos().y;
					if (renderPos.y > windowStart && renderPos.y < windowStart + ImGui::GetWindowHeight()) {

						// render autocomplete tooltip
						ImGui::SetNextWindowPos(renderPos);
						_CreateAutocompleteTooltip(lastWord, lastChar.compare(":") == 0);
					}
				}
			}
		}
	}
}

void TurbineTextEditor::_CreateAutocompleteTooltip(std::string name, bool isMethod) {

	ImGui::Begin("AUTOCOMPLETE", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
	
	// does name exist in dictionary
	if (dictionary.find(name) != dictionary.end()) {

		std::vector<std::string>& list = (isMethod ? dictionary[name].methods : dictionary[name].variables);

		for (std::string& tag : list) {

			ImGui::Text(tag.c_str());
		}
	}
	ImGui::End();
}