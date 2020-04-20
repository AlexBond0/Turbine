#include "Object3DUI.h"



Object3DUI::Object3DUI()
{
}


Object3DUI::~Object3DUI()
{
}

void Object3DUI::Render() {

	ImGui::Begin("Object");
	ImGui::Text(("ID : " + std::string(object->GetName())).c_str());
	ImGui::Separator();
	ImGui::SliderFloat3("world position", &object->GetWorldPosVec()[0], -1.0, 1.0);
	ImGui::SliderFloat3("local position", &object->GetLocalPosVec()[0], -1.0, 1.0);
	ImGui::SliderFloat4("orientation", &object->GetOrientationQuat()[0], -1.0, 1.0);
	ImGui::SliderFloat4("rotation", &object->GetRotationQuat()[0], -1.0, 1.0);
	ImGui::Separator();
	ImGui::Checkbox("is Active", &object->isActive);
	ImGui::Checkbox("Use Texture", &object->useTexture);
	ImGui::Checkbox("Use Lighing", &object->useLight);
	ImGui::Separator();
	ImGui::Checkbox("Show Highlighted", &object->isHighlighted);
	ImGui::ColorEdit4("Ambient", (&object->GetAmbient()->rgba)[0]);
	ImGui::ColorEdit4("Diffuse", (&object->GetDiffuse()->rgba)[0]);
	ImGui::ColorEdit4("Specular", (&object->GetSpecular()->rgba)[0]);
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