#include "LightManager.h"



LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}


void LightManager::AddLight(Light* light) {

	_allLights[light->GetName()] = light;
}

std::map<std::string, Light*>* LightManager::GetAllLights() {

	return &_allLights;
}

void LightManager::RenderLights(RenderingContext& rcontext) {

	// directional light
	if (_currentDirLight != nullptr) {


	}

	PointLight* sun = dynamic_cast<PointLight*>(_allLights["sun"]);

	rcontext.liveShader->SetVector("pointLight.position", *sun->GetLocalPosVec());
	rcontext.liveShader->SetColor("pointLight.ambient", sun->ambient.rgba);
	rcontext.liveShader->SetColor("pointLight.diffuse", sun->diffuse.rgba);
	rcontext.liveShader->SetColor("pointLight.specular", sun->specular.rgba);
	rcontext.liveShader->SetFloat("pointLight.specStrength", sun->specularStrength);
	rcontext.liveShader->SetFloat("pointLight.constant", 1.0f);
	rcontext.liveShader->SetFloat("pointLight.linear", 0.09);
	rcontext.liveShader->SetFloat("pointLight.quadratic", 0.032);
}

bool LightManager::SetCurrentDirLight(std::string light) {

	// no light in lightmanager
	if (_allLights.find(light) == _allLights.end())
		return false;

	// if the light is a directional light
	else if (_allLights[light]->lightType == LightType::DIRECTION) {

		_currentDirLight = _allLights[light];
		return true;
	}

	// not a directional light
	else
		return false;
}