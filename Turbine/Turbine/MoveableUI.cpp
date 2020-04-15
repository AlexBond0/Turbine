#include "MoveableUI.h"



MoveableUI::MoveableUI()
{
}


MoveableUI::~MoveableUI()
{
}

void MoveableUI::Render() {

	std::string id = "ID : NULL";
	if (object->GetName())
		id = "ID : " + std::string(object->GetName());

	ImGui::Begin("Object");
	ImGui::Text(id.c_str());
	ImGui::Separator();
	ImGui::SliderFloat3("world position", &object->GetWorldPosVec()[0], -1.0, 1.0);
	ImGui::SliderFloat3("local position", &object->GetLocalPosVec()[0], -1.0, 1.0);
	ImGui::SliderFloat4("orientation", &object->GetOrientationQuat()[0], -1.0, 1.0);
	ImGui::SliderFloat4("rotation", &object->GetRotationQuat()[0], -1.0, 1.0);
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