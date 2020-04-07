#pragma once

#include <stdlib.h>
#include <time.h>

#include "RenderingContext.h"
#include "Model3D.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "RideAnimation.h"
#include "InstancedObject.h"
#include "Particle.h"
//
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"

// The Rendering enviroment
class Scene
{
public:

	std::map<std::string, Model3D*> models;		// the map of all model pointers
	std::vector<Model3D*> modelsToDraw;			// the vector of models to be drawn

	std::map<std::string, Object3D*> objects;	// the map of all object pointers
	std::vector<Object3D*> objectsToDraw;		// the vector of objects to be drawn

	std::vector<Particle*> particleSystems;		// the vector of objects to be drawn

	bool rideCam = false;						// flag denoting if POV cam is in use

	Camera camera;								// the main curveball camera
	CameraPositioner camPOV;					// the POV camera positioner

	Light light;								// Light object for the sun

	RideAnimation animator;						// the model animator

	Texture textures;							// the set of loaded textures

	bool pov = false;							// is the POV camera enabled



	Scene();
	~Scene();

	void Render(RenderingContext rcontext);
	void Setup();

	void OnTimer(RenderingContext rcontext, long timePassed);

	void ToggleAnimation();
	void ToggleCamera();
	void ToggleDayNight();

	void RotateCamera(float x, float y, bool thispov);

	Object3D* CreateObject(std::string name);
	Object3D* CreateObject(Object3D* copy, std::string newName);

private:	

	bool sceneLoaded;						// has the scene been loaded

	bool isDoingStandardAnimation = false;	// has the standard animation been called
	bool isDay = true;						// the current day/night status

	void _LoadRide();
	void _GenerateTrees();
	void _GenerateSeats();
	void _GenerateParticles();
	void _UnpackObjects(Model3D* model);

	// void TestImGui();
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
		camera.DefaultPOVCam();

	else
		camera.DefaultSceneCam();
}

// toggle day/night
inline void Scene::ToggleDayNight() {

	isDay = !isDay;

	if (isDay) {

		objects["skybox"]->SetTexture(textures.id["skybox"]);
		light.CreateSun();
	}
	else {

		objects["skybox"]->SetTexture(textures.id["skybox-night"]);
		light.CreateMoon();
	}
}

// preform the camera rotation
inline void Scene::RotateCamera(float x, float y, bool thispov) {

	camera.RotateCam(x, y, thispov);
}

// create a new object and store the object in the objects[] map
inline Object3D* Scene::CreateObject(std::string name) {

	objects[name] = new Object3D();
	return objects[name];
}

// create a new named object and store the object in the objects[] map
inline Object3D* Scene::CreateObject(Object3D* copy, std::string newName) {

	objects[newName] = new Object3D(copy, newName);
	return objects[newName];
}
