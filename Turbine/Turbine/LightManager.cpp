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