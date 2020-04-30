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

	_worldPos		= *currentEntity->GetWorldPosVec();
	_localPos		= *currentEntity->GetLocalPosVec();
	_translation	= *currentEntity->GetTranlationVec();
	_scale			= *currentEntity->GetScaleVec();
	_up				= *currentEntity->GetUpVec();
	_orientation	= currentEntity->GetOrientationQuat();
	_rotation		= currentEntity->GetRotationQuat();

	ImGui::NewLine();
	ImGui::TextColored(entityCol, "Entity");
	ImGui::Separator();

	if (ImGui::DragFloat3("World position", &_worldPos[0], 0.1)) 
		currentEntity->SetWorldPos(_worldPos);
	
	if (ImGui::DragFloat3("Local position", &_localPos[0], 0.1))
		currentEntity->SetLocalPos(_localPos);

	if (ImGui::DragFloat3("Translation", &_localPos[0], 0.1))
		currentEntity->SetTranslation(_translation);

	if (ImGui::DragFloat3("Local up", &_up[0], 0.1))
		currentEntity->SetUp(_up);

	ImGui::Separator();

	if (ImGui::DragFloat3("Scale", &_scale[0], 0.1))
		currentEntity->SetScale(_scale);

	ImGui::Separator();

	if (ImGui::DragFloat4("Orientation", &_orientation[0], 0.1))
		currentEntity->SetOrientation(_orientation);

	if (ImGui::DragFloat4("Rotation", &_rotation[0], 0.1))
		currentEntity->SetRotation(_rotation);
}

void EntityUI::_RenderObj() {

	_RenderEntity();
	
	ImGui::NewLine();
	ImGui::TextColored(objCol, "Object3D");
	ImGui::Separator();

	Object3D* object = dynamic_cast<Object3D*>(currentEntity);


	ImGui::Columns(2);

	
	ImGui::Checkbox("Use Texture", &object->useTexture);
	ImGui::Checkbox("Use Lighing", &object->useLight);
	ImGui::Checkbox("Is Transparent", &object->isTransparent);

	ImGui::NextColumn();

	ImGui::Checkbox("Show Highlighted", &object->isHighlighted);
	ImGui::Checkbox("Is Active", &object->isActive);
	ImGui::Checkbox("Is Locally Active", &object->isLocallyActive);

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

	ImGui::Checkbox("Is Particle Active", &particle->active);
	ImGui::DragFloat("Partcle weight", &particle->profile.weight);
	ImGui::DragInt("Particle spread", &particle->profile.spread);
	ImGui::DragFloat("Partile life", (float*)(&particle->profile.life));

	ImGui::DragFloat4("Emitter position", &particle->profile.camPosition[0], 0.1);
	ImGui::DragFloat4("Emitter speed", &particle->profile.speed[0], 0.1);
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

	ImGui::Checkbox("Show Camera in scene", &camera->cameraUI.showUIObject);

	ImGui::Separator();
	ImGui::Text("Aspect Ratio : ");
	ImGui::SameLine();
	ImGui::TextColored(value, (std::to_string(camera->fAspect).c_str()));

	
	bool fa = ImGui::DragFloat("Feild of View", &camera->fFovy, 0.01);
	bool fb = ImGui::DragFloat("Near clipping plane", &camera->fZNear, 0.1);
	bool fc = ImGui::DragFloat("Far clipping plane", &camera->fZFar, 0.1);

	if (fa || fb || fc) {

		camera->fDirty = true;
	}

	ImGui::Separator();
	ImGui::DragFloat3("Camera position", &(*camera->GetLocalPosVec())[0], 0.01);
	ImGui::DragFloat3("Camera target", &(*camera->GetTarget())[0], 0.01);
	ImGui::DragFloat3("Camera up", &(*camera->GetUpVec())[0], 0.01);
}

void EntityUI::_RenderLight() {

	_RenderEntity();

	ImGui::NewLine();
	ImGui::TextColored(lightCol, "Light");
	ImGui::Separator();

	Light* light = dynamic_cast<Light*>(currentEntity);

	ImGui::Checkbox("Show Light in scene", &light->lightUI.showUIObject);

	ImGui::Separator();
	ImGui::ColorEdit4("Ambient", &light->ambient.rgba[0]);
	ImGui::ColorEdit4("Diffuse", &light->diffuse.rgba[0]);
	ImGui::ColorEdit4("Specular", &light->specular.rgba[0]);
}