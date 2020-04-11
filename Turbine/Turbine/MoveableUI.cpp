#include "MoveableUI.h"



MoveableUI::MoveableUI()
{
}


MoveableUI::~MoveableUI()
{
}

void MoveableUI::Render() {

	ImGui::Begin("Light");
	ImGui::SliderFloat("Dir X", &light->direction.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Dir Y", &light->direction.y, -1.0f, 1.0f);
	ImGui::SliderFloat("Dir Z", &light->direction.z, -1.0f, 1.0f);
	ImGui::Separator();
	ImGui::Text("FPS : (%f)", (float)(1 / timePassed));
	ImGui::End();
}



FramesUI::FramesUI()
{
}


FramesUI::~FramesUI()
{
}

void FramesUI::Render() {

	std::string title = "FPS : " + std::to_string(1 / timePassed);
	float p = 7;

	ImGui::Begin("Eh"); // title.c_str());
	ImGui::SliderFloat("Pos X", &p, -30.0f, 30.0f);
	ImGui::End();
}