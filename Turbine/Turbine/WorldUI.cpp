#include "WorldUI.h"



WorldUI::WorldUI() {

	_frames = std::vector<float>(100, 0.0f);
}


WorldUI::~WorldUI()
{
}

void WorldUI::Render() {

	ImGui::Begin("World");

	_RenderFPS();

	ImGui::Text("Rendering");
	ImGui::RadioButton("Object", &renderer, 0); ImGui::SameLine();
	ImGui::RadioButton("Smooth", &renderer, 1);

	switch (renderer) {
		case 0: rcontext->SetLiveShader("object"); break;
		case 1: rcontext->SetLiveShader("smooth"); break;
	}

	ImGui::BeginChild("Entites", ImVec2(0, 0), true);

	ImGui::Columns(3);
	ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() - 145);
	ImGui::SetColumnWidth(1, 80);
	ImGui::SetColumnWidth(2, 65);

	ImGui::Text("Entites");
	ImGui::NextColumn();
	ImGui::Text("Type");
	ImGui::NextColumn();
	ImGui::Text("Select");
	ImGui::NextColumn();
	ImGui::Separator();

	// for each entity at base
	for (auto const& base : world->GetAllBaseEntities()) {

		_RenderEntity(base.second);
	}

	ImGui::EndChild();

	ImGui::Columns(1);
	ImGui::End();
}

void WorldUI::_RenderEntity(Entity* entity) {

	// show entity activity
	_ColourActive(entity);

	// if there are children entites
	if (entity->GetChildren().size() > 0) {

		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_DefaultOpen;
		bool treeOpen = ImGui::TreeNodeEx(entity->GetName().c_str(), node_flags);
		_ColourFinish();

		_ShowSelectedEntity(entity);

		_RenderEntityRow(entity);

		_RenderSelectorButton(entity);
		
		if (treeOpen) {

			for (Entity* e : entity->GetChildren())
				_RenderEntity(e);

			ImGui::TreePop();
		}

	}
	else {

		ImGui::Text(entity->GetName().c_str());
		_ColourFinish();

		_ShowSelectedEntity(entity);
		
		_RenderEntityRow(entity);

		_RenderSelectorButton(entity);
	}

}

void WorldUI::_ShowSelectedEntity(Entity* entity) {

	if (entity == world->currentSelectedEntity) {

		ImGui::PushStyleColor(ImGuiCol_Text, selected);
		ImGui::SameLine();
		ImGui::Text("[S]");
		ImGui::PopStyleColor();
	}
}

void WorldUI::_RenderEntityRow(Entity* entity) {

	// entity type
	ImGui::NextColumn();

	ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	std::string name = "UNKNOWN";

	switch (entity->GetEntityType()) {

		case EntityType::OBJ: {
			color = objCol;
			name = "Object3D";
			break;
		}

		case EntityType::OBJ_INSTANCED: {
			color = insanceCol;
			name = "Instanced";
			break;
		}

		case EntityType::OBJ_PARTICLE_SYS: {
			color = particleCol;
			name = "Particle";
			break;
		}

		case EntityType::OBJ_PRIMITIVE: {
			color = primitiveCol;
			name = "Primitive";
			break;
		}

		case EntityType::CAMERA: {
			color = cameraCol;
			name = "Camera";
			break;
		}

		case EntityType::LIGHT: {

			Light* light = dynamic_cast<Light*>(entity);

			switch (light->lightType) {

				case LightType::DIRECTION: {
			
					color = dirLightCol;
					name = "D";
					break;
				}

				case LightType::POINT: {

					color = pointLightCol;
					name = "P";
					break;
				}

				case LightType::SPOT: {

					color = spotLightCol;
					name = "S";
					break;
				}
			}

			ImGui::TextColored(color, name.c_str());
			ImGui::SameLine();

			color = lightCol;
			name = "Light";

			break;
		}
	}
	ImGui::TextColored(color, name.c_str());

	ImGui::NextColumn();
}

void WorldUI::_RenderSelectorButton(Entity* entity) {

	// ImGui seems to use unique naming for access, so construct named button
	if (ImGui::SmallButton(("       " + entity->GetName() + "btn").c_str())) {

		world->currentSelectedEntity = entity;
	}

	// ImGui::SameLine();
	ImGui::NextColumn();
}

void WorldUI::_RenderFPS() {

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
}

void WorldUI::_ColourActive(Entity* entity) {

	ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	switch (entity->GetEntityType()) {

	case EntityType::OBJ: 
	case EntityType::OBJ_INSTANCED:
	case EntityType::OBJ_PARTICLE_SYS:
	case EntityType::OBJ_PRIMITIVE:
	{
	
		Object3D* obj = dynamic_cast<Object3D*>(entity);

		if (!obj->isActive)
			color = isNotActive;

		else if (!obj->isLocallyActive)
			color = isNotLocallyActive;

		else if (!obj->IsGloballyActive())
			color = isNotGloballyActive;

		break;
	}

	}

	ImGui::PushStyleColor(ImGuiCol_Text, color);
}

void WorldUI::_ColourFinish() {

	ImGui::PopStyleColor();
}