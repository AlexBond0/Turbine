#include "WorldUI.h"



WorldUI::WorldUI()
{
}


WorldUI::~WorldUI()
{
}

void WorldUI::Render() {

	ImGui::Begin("World");

	//ImGui::ColorEdit4("objCol", &objCol.x);
	//ImGui::ColorEdit4("insanceCol", &insanceCol.x);
	//ImGui::ColorEdit4("particleCol", &particleCol.x);
	//ImGui::ColorEdit4("primitiveCol", &primitiveCol.x);
	//ImGui::ColorEdit4("cameraCol", &cameraCol.x);
	//ImGui::Separator();

	ImGui::Text("Rendering");
	ImGui::RadioButton("Object", &renderer, 0); ImGui::SameLine();
	ImGui::RadioButton("Smooth", &renderer, 1);

	switch (renderer) {
		case 0: rcontext->SetLiveShader("object"); break;
		case 1: rcontext->SetLiveShader("smooth"); break;
	}

	ImGui::Separator();

	ImGui::Columns(3);
	ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() - 140);
	ImGui::SetColumnWidth(1, 80);
	ImGui::SetColumnWidth(2, 60);

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

	ImGui::Columns(1);
	ImGui::End();
}

void WorldUI::_RenderEntity(Entity* entity) {

	// if there are children entites
	if (entity->GetChildren().size() > 0) {

		// create treenode
		if (ImGui::TreeNode(entity->GetName().c_str())) {

			_RenderEntityRow(entity);

			// render selector button for entity
			_RenderSelectorButton(entity);

			for (Entity* e : entity->GetChildren())
				_RenderEntity(e);

			ImGui::TreePop();
		}
		else {

			_RenderEntityRow(entity);

			// render selector button for entity
			_RenderSelectorButton(entity);
		}

	}
	else {

		ImGui::Text(entity->GetName().c_str());
		
		_RenderEntityRow(entity);

		_RenderSelectorButton(entity);
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