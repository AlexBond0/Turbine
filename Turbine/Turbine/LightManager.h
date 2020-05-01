#pragma once

#include "Light.h"

#define MAX_LIGHTS = 8;

class LightManager
{
public:
	LightManager();
	~LightManager();

	void AddLight(Light* light);
	std::map<std::string, Light*>* GetAllLights();

	bool SetCurrentDirLight(std::string light);

	void RenderLights(RenderingContext& rcontext);

private:

	std::map<std::string, Light*> _allLights;

	Light* _currentDirLight = nullptr;

	int _activePointLights;
	int _activeSpotLights;

};

