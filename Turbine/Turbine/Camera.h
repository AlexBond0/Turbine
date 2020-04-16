#pragma once

#include "Matrix.h"
#include "RenderingContext.h"
#include "Object3D.h"
#include "RenderingContext.h"

#include <vector>

// struct to return data from a picked object
struct PickObject {

	bool		hasBeenPicked = false;  // has the object been picked
	Object3D*	object;					// reference to the object picked
	float		distance = FLT_MAX;		// distacne along the pick ray the object was picked from
};

class Camera
	: public Entity
{

public:

	// seperate from element position
	glm::vec3 camPosition;	// XYZ of camera position
	glm::vec3 camTarget;	// XYZ of camera target
	glm::vec3 camUp;		// XYZ of camera up

	bool moveable	= true;

	Camera();
	~Camera();

	// calcualte a picking ray from the scene
	glm::vec3 CalculatePickRay(float mouseX, float mouseY, float windW, float windH);
	PickObject ObjectPicked(Object3D* object, glm::vec3 pickingRay);
	PickObject GetPickedObject(std::map<std::string, Object3D*>* objects, glm::vec3 pickingRay);

	void SetPosition(glm::vec3 newPosition);
	void SetTarget(glm::vec3 newTarget);
	void SetUp(glm::vec3 newUp);

	void DefaultSceneCam();
	void DefaultPOVCam();

	void MoveCam(int newX, int newY);
	void RotateCam(int newX, int newY, bool arcballCam);
	void FinishMovement();
	void FocusCam(int focusDelta);

	void LookThrough(RenderingContext& rcontext);

	// our FOV is 60 degrees 
	float		fFovy	= (60.0f*(float)M_PI / 180.0f);

	float		fZNear	= 0.01f;		// near clipping plane
	float		fZFar	= 1000.0f;		// far clipping plane
	float		fAspect;				// aspect ratio

private:
	bool	_prevValuesDirty = true;
	int		_prevX = -1;
	int		_prevY = -1;

	void	_MoveCam(glm::vec3 movement);
};

// small class for positioning a camera according to a list of object transformations
// very broken, will be replaced with entity based systam
class CameraPositioner {

public:

	std::vector<Object3D*> objList;

	float offset[3];

	void PositionCamera(RenderingContext& rcontext, Camera cam);

private:

	glm::mat4 offsetMatrix;

	glm::vec4 relPosition;
	glm::vec4 relTarget;
	glm::vec4 relUp;
};