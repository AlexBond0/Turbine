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
		case EntityType::OBJ_INSTANCED: _RenderInstance();  break;
		case EntityType::OBJ_PARTICLE_SYS: _RenderParticle();  break;
		case EntityType::OBJ_PRIMITIVE: _renderPrimitive();  break;
		case EntityType::CAMERA: _RenderCamera();  break;
		case EntityType::LIGHT: _RenderLight();  break;

		}

		ImGui::End();
	}
}

void EntityUI::_RenderEntity() {

	glm::vec3 worldPos = currentEntity->GetWorldPosVec();
	glm::vec3 localPos = currentEntity->GetLocalPosVec();
	glm::vec3 up = currentEntity->GetUp();
	glm::quat orientation = currentEntity->GetOrientationQuat();
	glm::quat rotation = currentEntity->GetRotationQuat();

	ImGui::NewLine();
	ImGui::TextColored(entityCol, "Entity");
	ImGui::Separator();

	ImGui::DragFloat3("World position", &worldPos[0], 0.1);
	ImGui::DragFloat3("Local position", &localPos[0], 0.1);
	ImGui::DragFloat3("Local up", &up[0], 0.1);
	ImGui::DragFloat4("Orientation", &orientation[0], 0.1);
	ImGui::DragFloat4("Rotation", &rotation[0], 0.1);

	currentEntity->SetWorldPos(worldPos);
	currentEntity->SetLocalPos(localPos);
	currentEntity->SetUp(up);
	currentEntity->SetOrientation(orientation);
	currentEntity->SetRotation(rotation);
}

void EntityUI::_RenderObj() {

	_RenderEntity();
	
	ImGui::NewLine();
	ImGui::TextColored(objCol, "Object3D");
	ImGui::Separator();

	Object3D* object = dynamic_cast<Object3D*>(currentEntity);


	ImGui::Columns(2);

	ImGui::Checkbox("Is Active", &object->isActive);
	ImGui::Checkbox("Use Texture", &object->useTexture);
	ImGui::Checkbox("Use Lighing", &object->useLight);

	ImGui::NextColumn();

	ImGui::Checkbox("Show Highlighted", &object->isHighlighted);
	ImGui::Checkbox("Is Transparent", &object->isTransparent);

	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::DragFloat("Specular level", object->GetSpecLevel(), 0.1f);
	ImGui::ColorEdit4("Ambient", (&object->GetAmbient()->rgba)[0]);
	ImGui::ColorEdit4("Diffuse", (&object->GetDiffuse()->rgba)[0]);
	ImGui::ColorEdit4("Specular", (&object->GetSpecular()->rgba)[0]);

}

void EntityUI::_RenderInstance() {

	_RenderObj();

	ImGui::NewLine();
	ImGui::TextColored(insanceCol, "Instanced Object");
	ImGui::Separator();

	InstancedObject* instance = dynamic_cast<InstancedObject*>(currentEntity);

	ImGui::Text("Instance count : ");
	ImGui::SameLine();
	ImGui::TextColored(value, (std::to_string(instance->GetInstanceCount())).c_str());
}

void EntityUI::_RenderParticle() {

	_RenderInstance();

	ImGui::NewLine();
	ImGui::TextColored(particleCol, "Particle System");
	ImGui::Separator();

	Particle* particle = dynamic_cast<Particle*>(currentEntity);

	ImGui::Checkbox("Is active", &particle->active);
	ImGui::DragFloat("Partcle weight", &particle->profile.weight);
	ImGui::DragInt("Particle spread", &particle->profile.spread);
	ImGui::DragFloat("Partile life", (float*)(&particle->profile.life));

	ImGui::DragFloat4("Emitter position", &particle->profile.camPosition[0], 0.1);
	ImGui::DragFloat4("Emitter speed", &particle->profile.speed[0], 0.1);
	ImGui::ColorEdit4("Particle colour", (&particle->profile.colour.rgba)[0]);

}

void EntityUI::_renderPrimitive() {

	_RenderObj();
}

void EntityUI::_RenderCamera() {

	_RenderEntity();

	ImGui::NewLine();
	ImGui::TextColored(cameraCol, "Camera");
	ImGui::Separator();

	Camera* camera = dynamic_cast<Camera*>(currentEntity);

	ImGui::Checkbox("Show Camera in scene", &camera->showUI);

	ImGui::Separator();
	ImGui::DragFloat3("Camera position", &camera->camPosition[0], 0.1);
	ImGui::DragFloat3("Camera target", &camera->camTarget[0], 0.1);
	ImGui::DragFloat3("Camera up", &camera->up[0], 0.1);
}

void EntityUI::_RenderLight() {

	_RenderEntity();

	ImGui::NewLine();
	ImGui::TextColored(lightCol, "Light");
	ImGui::Separator();
}