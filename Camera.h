#pragma once

#include "Matrix.h"
#include "RenderingContext.h"
#include "Object3D.h"

#include <vector>

class Camera {

public:

	float position[3];	// XYZ of camera position
	float target[3];	// XYZ of camera target
	float up[3];		// XYZ of camera up

	bool moveable	= true;

	Camera();
	~Camera();

	// calcualte a picking ray from the scene
	float* CalculatePickRay(float windX, float windY, float windH, float windV);

	void SetPosition(float x, float y, float z);
	void SetTarget(float x, float y, float z);
	void SetUp(float x, float y, float z);

	void DefaultSceneCam();
	void DefaultPOVCam();

	void MoveCam(int newX, int newY);
	void RotateCam(int newX, int newY, bool arcballCam);
	void FinishMovement();
	void FocusCam(int focusDelta);

private:
	bool	_prevValuesDirty = true;
	int		_prevX = -1;
	int		_prevY = -1;

	void	_MoveCam(float dx, float dy, float dz);
};

// small class for positioning a camera according to a list of object transformations
class CameraPositioner {

public:

	std::vector<Object3D*> objList;

	float offset[3];

	void PositionCamera(RenderingContext& rcontext, Camera cam);

private:

	float offsetMatrix[16];

	float relPosition[3];
	float relTarget[3];
	float relUp[3];
};