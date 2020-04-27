#include "EntityUI.h"



EntityUI::EntityUI()
{
}


EntityUI::~EntityUI()
{
}


void EntityUI::Render() {


	if (world->currentSelectedEntity != nullptr) {

		currentEntity = world->currentSelectedEntity;

		ImGui::Begin("Entity");
		ImGui::Text(("Name : " + std::string(currentEntity->GetName())).c_str());
		ImGui::Separator();

		switch (currentEntity->GetEntityType()) {

		case EntityType::OBJ: _RenderObj();  break;
		case EntityType::OBJ_INSTANCED: _RenderObj();  break;
		case EntityType::OBJ_PARTICLE_SYS: _RenderObj();  break;
		case EntityType::OBJ_PRIMITIVE: _RenderObj();  break;
		// case EntityType::CAMERA: _RenderObj();  break;
		// case EntityType::LIGHT: _RenderObj();  break;

		}

		ImGui::End();
	}
}

void EntityUI::_RenderEntity() {

	ImGui::SliderFloat3("World position", &currentEntity->GetWorldPosVec()[0], -1.0, 1.0);

	glm::vec3 worldPos = currentEntity->GetWorldPosVec();
	glm::vec3 localPos = currentEntity->GetLocalPosVec();
	glm::vec3 up = currentEntity->GetUp();
	glm::quat orientation = currentEntity->GetOrientationQuat();
	glm::quat rotation = currentEntity->GetRotationQuat();

	ImGui::SliderFloat3("World position", &worldPos[0], -1.0, 1.0);
	ImGui::SliderFloat3("Local position", &localPos[0], -1.0, 1.0);
	ImGui::SliderFloat3("Local up", &up[0], -1.0, 1.0);
	ImGui::SliderFloat4("Orientation", &orientation[0], -1.0, 1.0);
	ImGui::SliderFloat4("Rotation", &rotation[0], -1.0, 1.0);

	currentEntity->SetWorldPos(worldPos);
	currentEntity->SetLocalPos(localPos);
	currentEntity->SetUp(up);
	currentEntity->SetOrientation(orientation);
	currentEntity->SetRotation(rotation);
}

void EntityUI::_RenderObj() {

	_RenderEntity();
	ImGui::Separator();

	Object3D* object = dynamic_cast<Object3D*>(currentEntity);

	ImGui::Separator();
	ImGui::Checkbox("Is Active", &object->isActive);
	ImGui::Checkbox("Use Texture", &object->useTexture);
	ImGui::Checkbox("Use Lighing", &object->useLight);
	ImGui::Separator();

	ImGui::Checkbox("Show Highlighted", &object->isHighlighted);
	ImGui::Checkbox("Is Transparent", &object->isTransparent);
	ImGui::ColorEdit4("Ambient", (&object->GetAmbient()->rgba)[0]);
	ImGui::ColorEdit4("Diffuse", (&object->GetDiffuse()->rgba)[0]);
	ImGui::ColorEdit4("Specular", (&object->GetSpecular()->rgba)[0]);

}

void _RenderInstance();
void _RenderParticle();
void _RenderCamera();
void _RenderLight();