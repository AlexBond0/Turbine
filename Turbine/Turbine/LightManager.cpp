#include "LightManager.h"



LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}


void LightManager::AddLight(Light* light) {

	_allLights[light->GetName()] = light;
	_sortableLights.push_back(SortableLight(light));
}

std::map<std::string, Light*>* LightManager::GetAllLights() {

	return &_allLights;
}

void LightManager::RenderLights(RenderingContext& rcontext, glm::vec3 cameraPos) {

	// directional light
	if (_currentDirLight != nullptr) {
		rcontext.liveShader->SetBool("u_usesDirLight", true);

		glm::vec3 dir = _currentDirLight->GetLightDirection();
		rcontext.liveShader->SetVector("dirLight.direction", dir);
		rcontext.liveShader->SetColor("dirLight.ambient", _currentDirLight->ambient.rgba);
		rcontext.liveShader->SetColor("dirLight.diffuse", _currentDirLight->diffuse.rgba);
		rcontext.liveShader->SetColor("dirLight.specular", _currentDirLight->specular.rgba);
		rcontext.liveShader->SetFloat("dirLight.specStrength", _currentDirLight->specularStrength);
	}
	else {

		rcontext.liveShader->SetBool("u_usesDirLight", false);
	}

	// sort lights
	for (SortableLight& light : _sortableLights)
		light.dist = glm::distance(light.light->GetWorldPosition(), cameraPos);

	std::sort(_sortableLights.begin(), _sortableLights.end());

	// point lights
	int index = 0;
	int lightCount = 0;
	PointLight* pointLight;
	while (lightCount < MAX_LIGHTS && index < _sortableLights.size()) {

		if (_sortableLights[index].light->lightType == LightType::POINT) {

			pointLight = dynamic_cast<PointLight*>(_sortableLights[index].light);

			std::string lightStr = "pointLights[" + std::to_string(lightCount) + "]";

			rcontext.liveShader->SetVector(lightStr+".position", *pointLight->GetLocalPosVec());
			rcontext.liveShader->SetColor(lightStr+".ambient", pointLight->ambient.rgba);
			rcontext.liveShader->SetColor(lightStr+".diffuse", pointLight->diffuse.rgba);
			rcontext.liveShader->SetColor(lightStr+".specular", pointLight->specular.rgba);
			rcontext.liveShader->SetFloat(lightStr+".specStrength", pointLight->specularStrength);
			rcontext.liveShader->SetFloat(lightStr+".constant", pointLight->constant);
			rcontext.liveShader->SetFloat(lightStr+".linear", pointLight->linear);
			rcontext.liveShader->SetFloat(lightStr+".quadratic", pointLight->quadratic);

			lightCount++;
		}

		index++;
	}
	rcontext.liveShader->SetInt("u_pointLightCount", lightCount);

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