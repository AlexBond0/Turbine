
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

//// calcualte a picking ray from the camrea
//float* Camera::CalculatePickRay(float windX, float windY, float windH, float windV) {
//
//	float pickRay[3] = { 0.0f, 0.0f, 0.0f };
//
//	// normalise x and y dimentions between [-1 and 1]
//	float normX = (windX / windH) * 2 - 1;
//	float normY = (windY / windV) * 2 - 1;
//
//	// create view vector
//	float view[3] = {
//		target[0] - position[0],
//		target[1] - position[1],
//		target[2] - position[2],
//	};
//	MyMatrix::Normalise3(view);
//
//	// create horz vector
//	float horz[3];
//	MyMatrix::CrossProduct3(view, up, horz);
//	MyMatrix::Normalise3(horz);
//
//	// create vert
//	float vert[3];
//	MyMatrix::CrossProduct3(horz, view, vert);
//	MyMatrix::Normalise3(vert);
//
//	return pickRay;
//}

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

	float direction[3] = {
			position[0] - target[0],
			position[1] - target[1],
			position[2] - target[2]
	};

	float focus = (float)(zoomDelta > 0 ? focusDelta : -focusDelta);

	if (MyMatrix::length(direction) > focusDelta || focus < 0) {

		// position -= glm::normalize(difference) * focus;
		MyMatrix::Normalise3(direction);
		MyMatrix::Scale(direction, focus);

		position[0] -= direction[0];
		position[1] -= direction[1];
		position[2] -= direction[2];
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
	MyMatrix::SetIdentity(offsetMatrix);

	// position the camera through all object relations
	for (int objIndex = 0; objIndex < objList.size(); objIndex++) {

		MyMatrix::Translate(
			offsetMatrix,
			(objList[objIndex]->GetLocalPosVec()).x,
			(objList[objIndex]->GetLocalPosVec()).y,
			(objList[objIndex]->GetLocalPosVec()).z
		);

		MyMatrix::RotateXYZ(
			offsetMatrix,
			(objList[objIndex]->GetRotationVec()).x,
			(objList[objIndex]->GetRotationVec()).y,
			(objList[objIndex]->GetRotationVec()).z
		);
	}

	// preform the last offset
	MyMatrix::Translate(
		offsetMatrix,
		offset[0],
		offset[1],
		offset[2]
	);
	 
	// set position using last column of the offsetMatrix
	relPosition[0] = offsetMatrix[12];
	relPosition[1] = offsetMatrix[13];
	relPosition[2] = offsetMatrix[14];

	// transform the camera's target matrix so the camera can still look around
	float direction[3] = { cam.target.x, cam.target.y, cam.target.z };
	MyMatrix::MultiplyVecByMatrix(direction, offsetMatrix);
	relTarget[0] = direction[0];
	relTarget[1] = direction[1];
	relTarget[2] = direction[2];
	MyMatrix::Normalise3(relTarget);

	// set up using y column of the offsetMatrix
	relUp[0] = offsetMatrix[4];
	relUp[1] = offsetMatrix[5];
	relUp[2] = offsetMatrix[6];

	// set the rcontex matricies
	MyMatrix::SetLookAt(
		glm::value_ptr(rcontext.viewmatrix),
		relPosition,
		relTarget,
		relUp
	);
}