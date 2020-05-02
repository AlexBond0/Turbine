#pragma once

#include "Matrix.h"
#include "RenderingContext.h"
#include "Object3D.h"
#include "Primitive.h"
#include "RenderingContext.h"
#include "World.h"
#include "UIPointer.h"

#include <vector>

// struct to return data from a picked object
struct PickObject {

	bool		hasBeenPicked = false;  // has the object been picked
	Object3D*	object;					// reference to the object picked
	float		distance = FLT_MAX;		// distacne along the pick ray the object was picked from
};

class World; // forward decleration

class Camera
	: public Entity
{

public:

	// ===================================================================
	// Constructors and Destructors

	Camera(std::string name);
	~Camera();

	bool moveable = true;

	// calcualte a picking ray from the scene
	glm::vec3 CalculatePickRay(float mouseX, float mouseY, float windW, float windH);
	PickObject ObjectPicked(Object3D* object, glm::vec3 pickingRay);
	PickObject GetPickedObject(EntityManager* entities, glm::vec3 pickingRay);


	// ===================================================================
	// Implemented for Entity class

	void OnRender(RenderingContext& rcontext);
	Object3D* OnPick();

	// ===================================================================
	// Methods

	void DefaultSceneCam();
	void DefaultPOVCam();

	void MoveCam(int newX, int newY);
	void RotateCam(int newX, int newY, bool arcballCam);
	void FinishMovement();
	void FocusCam(int focusDelta);

	void LookThrough(RenderingContext& rcontext);

	void CalculateProjection(RenderingContext& rcontext, int width, int height);
	void CalculateProjection(RenderingContext& rcontext);

	bool		fDirty = false;

	float		fFovy	= (60.0f*(float)M_PI / 180.0f); // feild of view

	float		fZNear	= 0.01f;		// near clipping plane
	float		fZFar	= 1000.0f;		// far clipping plane
	float		fAspect;				// aspect ratio

	float		focusDelta = 0.3;		// rate of camera focus

	UIPointer cameraUI;

private:
	bool	_prevValuesDirty = true;
	int		_prevX = -1;
	int		_prevY = -1;
};