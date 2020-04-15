#include "CameraUI.h"



CameraUI::CameraUI() {

	_frames = std::vector<float>(100, 0.0f);
}


CameraUI::~CameraUI()
{
}

void CameraUI::Render() {

	ImGui::Begin("Camera");

	_currentfps = (float)(1 / timePassed);
	_frames.push_back(_currentfps);
	assert(!_frames.empty());
	_frames.erase(_frames.begin());

	ImGui::Text("FPS : (%f)", _currentfps);
	ImGui::PlotLines(
		"Frame Times", 
		&_frames[0],
		_frames.size(),
		100,
		NULL,
		0.0f,
		80.0f,
		ImVec2(0, 80)
	);

	ImGui::Separator();
	ImGui::SliderFloat3("position", &camera->position[0], -10.0, 10.0);
	ImGui::SliderFloat3("target", &camera->target[0], -7.0, 7.0);
	ImGui::SliderFloat3("up", &camera->up[0], -7.0, 7.0);
	ImGui::End();
}