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
	ImGui::Checkbox("Show Camera in scene", &camera->showUI);

	ImGui::Separator();
	ImGui::SliderFloat3("position", &camera->camPosition[0], -range, range);
	ImGui::SliderFloat3("target", &camera->camTarget[0], -range, range);
	ImGui::SliderFloat3("up", &camera->camUp[0], -range, range);
	ImGui::End();
}