#include "TurbineTextEditor.h"

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

			// is the last char an accessor
			if (lastChar.compare(":") == 0) {

				auto it = mLanguageDefinition.mIdentifiers.find(lastWord);
				if (it != mLanguageDefinition.mIdentifiers.end())
				{

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
						_CreateAutocompleteTooltip();
					}
				}
			}
		}
	}
}

void TurbineTextEditor::_CreateAutocompleteTooltip() {

	ImGui::Begin("AUTOCOMPLETE", NULL, ImGuiWindowFlags_Tooltip | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::Text("AUTOCOMPLETE");
	ImGui::End();
}