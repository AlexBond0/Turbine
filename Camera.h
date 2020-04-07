#pragma once

#include "Matrix.h"
#include "RenderingContext.h"
#include "Object3D.h"

#include <vector>

class Camera {

public:

	glm::vec3 position;	// XYZ of camera position
	glm::vec3 target;	// XYZ of camera target
	glm::vec3 up;		// XYZ of camera up

	bool moveable	= true;

	Camera();
	~Camera();

	// calcualte a picking ray from the scene
	// float* CalculatePickRay(float windX, float windY, float windH, float windV);

	void SetPosition(glm::vec3 newPosition);
	void SetTarget(glm::vec3 newTarget);
	void SetUp(glm::vec3 newUp);

	void DefaultSceneCam();
	void DefaultPOVCam();

	void MoveCam(int newX, int newY);
	void RotateCam(int newX, int newY, bool arcballCam);
	void FinishMovement();
	void FocusCam(int focusDelta);

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