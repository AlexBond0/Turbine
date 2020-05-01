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
	if (_currentDirLight != nullptr && _currentDirLight->isActive) {
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
	Light* sortLight;
	while (lightCount < MAX_LIGHTS && index < _sortableLights.size()) {

		sortLight = _sortableLights[index].light;

		if (sortLight->lightType == LightType::POINT && sortLight->isActive) {

			pointLight = dynamic_cast<PointLight*>(_sortableLights[index].light);

			std::string lightStr = "pointLights[" + std::to_string(lightCount) + "]";

			glm::vec3 pos = pointLight->GetWorldPosition();
			rcontext.liveShader->SetVector(lightStr+".position", pos);

			rcontext.liveShader->SetFloat(lightStr + ".constant", pointLight->constant);
			rcontext.liveShader->SetFloat(lightStr + ".linear", pointLight->linear);
			rcontext.liveShader->SetFloat(lightStr + ".quadratic", pointLight->quadratic);

			rcontext.liveShader->SetColor(lightStr+".ambient", pointLight->ambient.rgba);
			rcontext.liveShader->SetColor(lightStr+".diffuse", pointLight->diffuse.rgba);
			rcontext.liveShader->SetColor(lightStr+".specular", pointLight->specular.rgba);
			rcontext.liveShader->SetFloat(lightStr+".specStrength", pointLight->specularStrength);

			lightCount++;
		}

		index++;
	}
	rcontext.liveShader->SetInt("u_pointLightCount", lightCount);

	// spot lights
	index = 0;
	lightCount = 0;
	SpotLight* spotLight;
	while (lightCount < MAX_LIGHTS && index < _sortableLights.size()) {

		sortLight = _sortableLights[index].light;

		if (sortLight->lightType == LightType::SPOT && sortLight->isActive) {

			spotLight = dynamic_cast<SpotLight*>(_sortableLights[index].light);

			std::string lightStr = "spotLights[" + std::to_string(lightCount) + "]";

			glm::vec3 pos = spotLight->GetWorldPosition();
			glm::vec3 dir = spotLight->GetLightDirection();
			rcontext.liveShader->SetVector(lightStr + ".position", pos);
			rcontext.liveShader->SetVector(lightStr + ".direction", dir);
			rcontext.liveShader->SetFloat(lightStr + ".cutOff", spotLight->CalculateInsideCutoff());
			rcontext.liveShader->SetFloat(lightStr + ".outerCutOff", spotLight->CalculateOutsideCutoff());

			rcontext.liveShader->SetColor(lightStr + ".ambient", spotLight->ambient.rgba);
			rcontext.liveShader->SetColor(lightStr + ".diffuse", spotLight->diffuse.rgba);
			rcontext.liveShader->SetColor(lightStr + ".specular", spotLight->specular.rgba);

			rcontext.liveShader->SetFloat(lightStr + ".specStrength", spotLight->specularStrength);
			rcontext.liveShader->SetFloat(lightStr + ".constant", spotLight->constant);
			rcontext.liveShader->SetFloat(lightStr + ".linear", spotLight->linear);
			rcontext.liveShader->SetFloat(lightStr + ".quadratic", spotLight->quadratic);

			lightCount++;
		}

		index++;
	}
	rcontext.liveShader->SetInt("u_spotLightCount", lightCount);

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