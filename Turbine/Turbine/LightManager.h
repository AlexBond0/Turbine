#pragma once

#include "Light.h"
#include <algorithm>

#define MAX_LIGHTS 4

struct SortableLight {

	SortableLight(Light* newLight) {

		light = newLight;
	}
	
	Light* light;
	float dist;

	bool operator<(SortableLight& that) {

		return this->dist < that.dist;
	}
};

class LightManager
{
public:
	LightManager();
	~LightManager();

	void AddLight(Light* light);
	std::map<std::string, Light*>* GetAllLights();

	bool SetCurrentDirLight(std::string light);

	void RenderLights(RenderingContext& rcontext, glm::vec3 cameraPos);

private:

	std::map<std::string, Light*> _allLights;

	std::vector<SortableLight> _sortableLights;


	Light* _currentDirLight = nullptr;

	int _activePointLights;
	int _activeSpotLights;

};

