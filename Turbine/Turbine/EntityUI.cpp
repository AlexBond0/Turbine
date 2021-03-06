#include "EntityUI.h"



EntityUI::EntityUI() {

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
		case EntityType::MODEL: _RenderModel();  break;
		case EntityType::OBJ_INSTANCED: _RenderInstance();  break;
		case EntityType::OBJ_PARTICLE_SYS: _RenderParticle();  break;
		case EntityType::OBJ_PRIMITIVE: _RenderPrimitive();  break;
		case EntityType::CAMERA: _RenderCamera();  break;
		case EntityType::LIGHT: _RenderLight();  break;

		}

		ImGui::End();
	}
}

void EntityUI::_RenderEntity() {

	_worldPos		= currentEntity->GetWorldPosition();
	_localPos		= *currentEntity->GetLocalPosVec();
	_translation	= *currentEntity->GetTranlationVec();
	_scale			= *currentEntity->GetScaleVec();
	_up				= *currentEntity->GetUpVec();

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

	_orientation.RenderQuaterion(
		"Orientation",
		currentEntity,
		[](Entity* e) -> glm::vec3 { return glm::eulerAngles(e->GetOrientationQuat()); }
	);

	_rotation.RenderQuaterion(
		"Rotation",
		currentEntity,
		[](Entity* e) -> glm::vec3 { return glm::eulerAngles(e->GetRotationQuat()); }
	);

	ImGui::Separator();
	ImGui::Columns(2);

	ImGui::Checkbox("Is Active", &currentEntity->isActive);
	ImGui::Checkbox("Is Locally Active", &currentEntity->isLocallyActive);

	_RenderStringVal(
		"Is Globally Active : ",
		(currentEntity->IsGloballyActive() ? "TRUE" : "FALSE")
	);

	ImGui::NextColumn();

	if (ImGui::Button("Look at entity")) {

		// camera can't look at itself
		if (world->GetActiveCamera() != currentEntity) {

			world->GetActiveCamera()->SetTarget(
				currentEntity->GetWorldPosition()
			);

			world->GetActiveCamera()->SetUp(glm::vec3(0.0, 1.0, 0.0));
		}
	}

	ImGui::Columns(1);
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
	ImGui::NextColumn();
	ImGui::Checkbox("Is Transparent", &object->isTransparent);
	ImGui::Checkbox("Render as Wireframe", &object->isWireframe);
	ImGui::Checkbox("Show Highlighted", &object->isHighlighted);
	ImGui::Columns(1);

	_RenderStringVal(
		"Has UV Coordinates : ",
		(object->vertices.HasUV() ? "TRUE" : "FALSE")
	);

	ImGui::Separator();
	ImGui::DragFloat("Specular level", object->GetSpecLevel(), 0.1f);
	ImGui::ColorEdit4("Ambient", (&object->GetAmbient()->rgba)[0]);
	ImGui::ColorEdit4("Diffuse", (&object->GetDiffuse()->rgba)[0]);
	ImGui::ColorEdit4("Specular", (&object->GetSpecular()->rgba)[0]);

}

void EntityUI::_RenderModel() {

	_RenderEntity();

	ImGui::NewLine();
	ImGui::TextColored(modelCol, "Model");
	ImGui::Separator();
}

void EntityUI::_RenderInstance() {

	_RenderObj();

	ImGui::NewLine();
	ImGui::TextColored(insanceCol, "Instanced Object");
	ImGui::Separator();

	InstancedObject* instance = dynamic_cast<InstancedObject*>(currentEntity);

	_RenderStringVal(
		"Instance count : ",
		std::to_string(instance->GetInstanceCount())
	);
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

void EntityUI::_RenderPrimitive() {

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

	_RenderStringVal(
		"Aspect Ratio : ",
		std::to_string(camera->fAspect)
	);

	
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
	ImGui::Checkbox("Light is Active", &light->isActive);

	ImGui::Separator();
	ImGui::ColorEdit4("Ambient", &light->ambient.rgba[0]);
	ImGui::ColorEdit4("Diffuse", &light->diffuse.rgba[0]);
	ImGui::ColorEdit4("Specular", &light->specular.rgba[0]);
	ImGui::DragFloat("Specular Strength", &light->specularStrength, 0.001);

	switch (light->lightType) {

		case LightType::DIRECTION : {

			ImGui::NewLine();
			ImGui::TextColored(dirLightCol, "Directional Light");
			ImGui::Separator();

			DirectionalLight* dirLight = dynamic_cast<DirectionalLight*>(currentEntity);

			ImGui::DragFloat3("Light Target", &(*dirLight->GetTarget())[0], 0.01);

			break;
		}

		case LightType::POINT: {

			ImGui::NewLine();
			ImGui::TextColored(pointLightCol, "Point Light");
			ImGui::Separator();

			PointLight* pointLight = dynamic_cast<PointLight*>(currentEntity);

			ImGui::DragFloat("Light Constant", &pointLight->constant, 0.01);
			ImGui::DragFloat("Light Linear", &pointLight->linear, 0.01);
			ImGui::DragFloat("Light Quadratic", &pointLight->quadratic, 0.01);

			break;
		}

		case LightType::SPOT: {

			ImGui::NewLine();
			ImGui::TextColored(spotLightCol, "Spotlight");
			ImGui::Separator();

			SpotLight* spotLight = dynamic_cast<SpotLight*>(currentEntity);

			ImGui::DragFloat3("Light Target", &(*spotLight->GetTarget())[0], 0.01);
			
			ImGui::DragFloat("Light Constant", &spotLight->constant, 0.01);
			ImGui::DragFloat("Light Linear", &spotLight->linear, 0.01);
			ImGui::DragFloat("Light Quadratic", &spotLight->quadratic, 0.01);

			ImGui::Separator();
			ImGui::DragFloat("Inside Cutoff", &spotLight->cutOff, 0.01);
			ImGui::DragFloat("Outside Cutoff", &spotLight->outerCutOff, 0.01);

			break;
		}
	}
}


void EntityUI::_RenderStringVal(std::string tag, std::string value) {

	ImGui::Text(tag.c_str());
	ImGui::SameLine();
	ImGui::TextColored(valueCol, value.c_str());
}


QuaterionHandler::QuaterionHandler() {

	_orientationFlip = true;
}

void QuaterionHandler::RenderQuaterion(std::string label, Entity* entity, FuncPtrQuat getNewQuatValue) {

	_quatAsEuler = getNewQuatValue(entity);

	_orientationSignX_old = (_quatAsEuler.x > 0);
	_orientationSignZ_old = (_quatAsEuler.z > 0);

	if (ImGui::DragFloat3(label.c_str(), &_quatAsEuler[0], (_orientationFlip ? 0.1 : -0.1))) {

		entity->SetOrientation(glm::quat(_quatAsEuler));
		_quatAsEuler = getNewQuatValue(entity); // &glm::eulerAngles(_currentEntity->GetOrientationQuat());

		_orientationSignX_new = (_quatAsEuler.x > 0);
		_orientationSignZ_new = (_quatAsEuler.z > 0);

		if (_orientationSignX_old != _orientationSignX_new && _orientationSignZ_old != _orientationSignZ_new)
			_orientationFlip = !_orientationFlip;

	}
}