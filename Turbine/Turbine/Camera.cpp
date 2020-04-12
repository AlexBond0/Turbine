
#include "Camera.h"


Camera::Camera() {

	DefaultSceneCam();
}


Camera::~Camera() {

}

// ================================================================

// set position of camera
void Camera::SetPosition(glm::vec3 newPosition) {

	position = newPosition;
}

// set the target of the camera
void Camera::SetTarget(glm::vec3 newTarget) {

	target = newTarget;
}

// set the up direction of the camera
void Camera::SetUp(glm::vec3 newUp) {

	up = newUp;
}

// ================================================================

void Camera::DefaultSceneCam() {

	SetTarget(glm::vec3(0.0f, 0.2f, 0.0f));
	SetPosition(glm::vec3(-0.8f, 0.5f, 1.0f));
	SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::DefaultPOVCam() {

	// these are more like offsets here as the camerapositioner handles the absolute position
	SetTarget(glm::vec3(0.0f, 0.0f, 1.0f));
	SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

// given a mouse xy and view xy, calculate a direction vertex for a picking ray
glm::vec3 Camera::CalculatePickRay(float mX, float mY, float vX, float vY) {

	// calculate reuseable h anv v values from camera information
	glm::vec3 h = glm::normalize(glm::cross(target, up));
	glm::vec3 v = glm::normalize(glm::cross(h, target));

	// use the near-clipping plane to get length values for h and v
	float vLength = tan(fFovy / 2) * fZNear;
	float hLength = vLength * (vX / vY);

	// scale h and v
	v *= vLength;
	h *= hLength;

	// calculate relative position of mouse on near clipping plane 
	float horzFactor = (mX - vX / 2) / (vX / 2);
	float vertFactor = (mY - vY / 2) / (vY / 2);

	// calculate position of mouse click on the fZNear plane
	glm::vec3 fZNearPos = position + (target * fZNear + h * horzFactor + v * vertFactor);

	// calculate direction from camera to fZNearPos
	return glm::normalize(fZNearPos - position);
}

// check if an object is picked with a given picking ray
bool Camera::ObjectPicked(Object3D* object, glm::vec3 pickingRay) {

	bool hasPicked = false;

	// implement here :P

	return hasPicked;
}

void Camera::RotateCam(int newX, int newY, bool arcballCam) {

	if (moveable) {

		// account for mouse moving when not dragging
		if (!_prevValuesDirty) {

			// get difference
			float diffX = float(newX - _prevX) / 100;
			float diffY = float(newY - _prevY) / 100;

			// create a camrea rotation matrix
			glm::mat4 cameraRotation = glm::mat4(1.0);
			
			// get the direction vector
			glm::vec3 direction = position - target;

			// preform pitch rotation
			glm::vec3 cross = glm::normalize(glm::cross(direction, up));

			// Rotate Mat4 By Vec3
			cameraRotation = glm::rotate(cameraRotation, -diffY, glm::cross(direction, up));

			// preform yaw rotation
			cameraRotation = glm::rotate(cameraRotation, diffX, glm::vec3(0, 1, 0));

			// calculate new direction
			direction = glm::vec3(glm::vec4(direction, 0) * cameraRotation);

			// set new position
			if (arcballCam) {

				position = (direction + target);
			}
			else {

				target = (position - direction);
			}

			// calculate up
			up = up * glm::mat3(cameraRotation);
		}

		// save previous values
		_prevX = newX;
		_prevY = newY;
		_prevValuesDirty = false;
	}
}

// move camrea by a set value for x and y
void Camera::MoveCam(int newX, int newY) {

	// account for mouse moving when not dragging
	if (!_prevValuesDirty) {

		// get difference
		float diffX = float(newX - _prevX) / 50.0f;
		float diffY = float(newY - _prevY) / 50.0f;

		// get the direction vector
		glm::vec3 direction = position - target;

		// calculate change horisontally
		glm::vec3 horz = glm::normalize(glm::cross(direction, up));

		// calculate change vertically
		glm::vec3 vert = glm::normalize(glm::cross(horz, direction));

		// scale the changes
		horz *= diffX;
		vert *= diffY;

		// apply the changes
		_MoveCam(horz + vert);
	}

	// save previous values
	_prevX = newX;
	_prevY = newY;
	_prevValuesDirty = false;
}

void Camera::FinishMovement() {

	_prevValuesDirty = true;
}


// change relationship between position & target
void Camera::FocusCam(int zoomDelta) {

	float focusDelta = 0.3;

	glm::vec3 direction = position - target;

	float focus = (float)(zoomDelta > 0 ? focusDelta : -focusDelta);

	if (glm::length(direction) > focusDelta || focus < 0) {

		direction = glm::normalize(direction);

		direction *= focus;

		position -= direction;
	}
}

// move cameras position and target in space
void Camera::_MoveCam(glm::vec3 movement) {

	position += movement;
	target += movement;
}

// position the camera through a set of object transformations
void CameraPositioner::PositionCamera(RenderingContext& rcontext, Camera cam) {

	// reset relposition
	offsetMatrix = glm::mat4(1.0);

	// position the camera through all object relations
	for (int objIndex = 0; objIndex < objList.size(); objIndex++) {

		offsetMatrix = glm::translate(offsetMatrix, objList[objIndex]->GetLocalPosVec());

		offsetMatrix = glm::rotate(
			offsetMatrix, 
			DEGS_TO_RADS(-objList[objIndex]->GetRotationVec().x), 
			glm::vec3(1, 0, 0)
		);
		offsetMatrix = glm::rotate(
			offsetMatrix,
			DEGS_TO_RADS(-objList[objIndex]->GetRotationVec().y),
			glm::vec3(0, 1, 0)
		);
		offsetMatrix = glm::rotate(
			offsetMatrix,
			DEGS_TO_RADS(-objList[objIndex]->GetRotationVec().z),
			glm::vec3(0, 0, 1)
		);

	}

	// preform the last offset
	offsetMatrix = glm::translate(
		offsetMatrix,
		glm::vec3(
			offset[0],
			offset[1],
			offset[2]
		)
	);

	 
	// set position using last column of the offsetMatrix
	relPosition[0] = offsetMatrix[3][0];
	relPosition[1] = offsetMatrix[3][1];
	relPosition[2] = offsetMatrix[3][2];

	// transform the camera's target matrix so the camera can still look around
	// float direction[3] = { cam.target.x, cam.target.y, cam.target.z };
	glm::vec4 direction = glm::vec4(cam.target, 1);

	// MyMatrix::MultiplyVecByMatrix(direction, offsetMatrix);
	direction = direction * offsetMatrix;


	relTarget = glm::normalize(direction);


	// set up using y column of the offsetMatrix
	relUp[0] = offsetMatrix[1][0];
	relUp[1] = offsetMatrix[1][1];
	relUp[2] = offsetMatrix[1][2];

	// set the rcontex matricies
	rcontext.viewmatrix = glm::lookAt(
		glm::vec3(relPosition),
		glm::vec3(relTarget),
		glm::vec3(relUp)
	);
}