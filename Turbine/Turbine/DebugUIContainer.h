#pragma once

#include "ImGui//imgui.h"

class DebugUIContainer {

public:
	virtual void Render() = 0;

protected:

	ImVec4 entityCol = ImVec4(0.5f, 1.0f, 0.5f, 1.0f);
	ImVec4 objCol = ImVec4(0.0f, 0.7941f, 1.0f, 1.0f);
	ImVec4 insanceCol = ImVec4(0.42f, 0.67f, 0.70f, 1.0f);
	ImVec4 particleCol = ImVec4(0.01f, 0.89f, 0.58f, 1.0f);
	ImVec4 primitiveCol = ImVec4(0.53f, 0.76f, 0.0f, 1.0f);
	ImVec4 cameraCol = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 modelCol = ImVec4(0.5f, 0.3f, 1.0f, 1.0f);

	ImVec4 lightCol = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
	ImVec4 dirLightCol = ImVec4(1.0f, 0.0f, 0.5f, 1.0f);
	ImVec4 pointLightCol = ImVec4(0.5f, 0.0f, 1.0f, 1.0f);
	ImVec4 spotLightCol = ImVec4(1.0f, 0.0f, 0.3f, 1.0f);

	ImVec4 valueCol = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 selected = ImVec4(0.5f, 0.0f, 1.0f, 1.0f);

	ImVec4 isNotGloballyActive = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	ImVec4 isNotLocallyActive = ImVec4(0.6f, 0.7f, 0.5f, 1.0f);
	ImVec4 isNotActive = ImVec4(0.7f, 0.5f, 0.5f, 1.0f);

	ImVec4 fogCol = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
};