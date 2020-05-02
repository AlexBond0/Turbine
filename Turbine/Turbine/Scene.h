#pragma once

#include <stdlib.h>
#include <time.h>

#include "RenderingContext.h"
#include "ModelLoader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "RideAnimation.h"
#include "InstancedObject.h"
#include "Particle.h"
#include "World.h"

// The Rendering enviroment
class Scene
{
public:

	World world;

	bool rideCam = false;						// flag denoting if POV cam is in use

	RideAnimation animator;						// the model animator

	Texture textures;							// the set of loaded textures

	bool pov = false;							// is the POV camera enabled

	Scene();
	~Scene();

	void Render(RenderingContext& rcontext);
	void Setup();

	void OnTimer(RenderingContext& rcontext, double timePassed);

	void ToggleAnimation();
	void ToggleCamera();
	void ToggleDayNight();

	void RotateCamera(float x, float y, bool thispov);

private:	

	bool sceneLoaded;						// has the scene been loaded

	bool isDoingStandardAnimation = false;	// has the standard animation been called
	bool isDay = true;						// the current day/night status

	void _ShadowPass(RenderingContext& rcontext);
	void _ObjectPass(RenderingContext& rcontext);
	void _TransparencyPass(RenderingContext& rcontext);

	void _LoadRide();
	void _GenerateTrees();
	void _GenerateSeats();
	void _GenerateParticles();
	void _GenerateLights();

};

// toggle the simple animation state
inline void Scene::ToggleAnimation() {

	isDoingStandardAnimation = !isDoingStandardAnimation;

	if (isDoingStandardAnimation)
		animator.QueueSpinStart();

	else
		animator.QueueSpinEnd();
}

// toggle the POV camera
inline void Scene::ToggleCamera() {

	pov = !pov;

	if (pov)
		world.SetActiveCamera("POV Camera");

	else
		world.SetActiveCamera("Default Camera");
}

// toggle day/night
inline void Scene::ToggleDayNight() {

	isDay = !isDay;

	if (isDay) {

		world.GetObject3D("skybox")->SetTexture(textures.id["skybox"]);
		dynamic_cast<Light*>(world.GetEntity("sun"))->CreateSun();
	}
	else {

		world.GetObject3D("skybox")->SetTexture(textures.id["skybox-night"]);
		dynamic_cast<Light*>(world.GetEntity("sun"))->CreateMoon();
	}
}

// preform the camera rotation
inline void Scene::RotateCamera(float x, float y, bool thispov) {

	world.GetActiveCamera()->RotateCam(x, y, thispov);
}