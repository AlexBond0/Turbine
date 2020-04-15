#include "CameraUI.h"



CameraUI::CameraUI()
{
}


CameraUI::~CameraUI()
{
}

void CameraUI::Render() {

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("position", &camera->position[0], -1.0, 1.0);
	ImGui::SliderFloat3("target", &camera->target[0], -1.0, 1.0);
	ImGui::SliderFloat3("up", &camera->up[0], -1.0, 1.0);
	ImGui::End();
}