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

void LightManager::RenderLights() {


}