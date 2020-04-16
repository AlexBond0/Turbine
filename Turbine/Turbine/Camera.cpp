
#include "Camera.h"


Camera::Camera() {

	DefaultSceneCam();
}


Camera::~Camera() {

}

// ================================================================

// set position of camera
void Camera::SetPosition(glm::vec3 newPosition) {

	camPosition = newPosition;
}

// set the target of the camera
void Camera::SetTarget(glm::vec3 newTarget) {

	camTarget = newTarget;
}

// set the up direction of the camera
void Camera::SetUp(glm::vec3 newUp) {

	camUp = newUp;
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
glm::vec3 Camera::CalculatePickRay(float mouseX, float mouseY, float windW, float windH) {

	// positions need to be between -1 and 1
	float clickPosX = mouseX / (windW  * 0.5f) - 1.0f;
	float clickPosY = mouseY / (windH * 0.5f) - 1.0f;

	// get diection of camera
	glm::vec3 direction = camPosition - camTarget;

	// calculate projection and view matrix for the camera
	glm::mat4 proj = glm::perspective(fFovy, fAspect, fZNear, fZFar);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f), -direction, camUp);

	// use matrices to calculate point clicked in sreen space
	glm::mat4 invVP = glm::inverse(proj * view);
	glm::vec4 screenPos = glm::vec4(clickPosX, -clickPosY, 1.0f, 1.0f);
	
	// get direction of ray
	glm::vec4 worldPos = invVP * screenPos;
	glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

	return dir;
}

// check if an object is picked with a given picking ray
PickObject Camera::ObjectPicked(Object3D* object, glm::vec3 pickingRay) {

	PickObject pickedObject;
	pickedObject.object = object;

	// object instancing needs sorting out at a later date
	if (object->IsInstanced())
		return pickedObject;

	// get the current translations of the object to refer to
	TranslationStack objectTranlation = object->GetWorldTranslation();

	glm::vec3 translatedPickRay = 
		glm::inverse(*(objectTranlation.GetCurrentModelMatrix())) 
		* glm::vec4(pickingRay, 0.0);

	glm::vec3 translatedCameraPosition = 
		glm::inverse(*(objectTranlation.GetCurrentModelMatrix()))
		* glm::vec4(camPosition, 1.0);

	glm::vec3* v1;
	glm::vec3* v2;
	glm::vec3* v3;
	glm::vec2 contact;
	float length;

	// check each polygon of object
	// will be replaced with maybe collision box instead
	for (Poly& polygon : *(object->polygons.GetVector())) {

		v1 = &object->vertices.GetPointUV(polygon.point[0])->vertex;
		v2 = &object->vertices.GetPointUV(polygon.point[1])->vertex;
		v3 = &object->vertices.GetPointUV(polygon.point[2])->vertex;

		// if collision occurs
		if (glm::intersectRayTriangle(
			translatedCameraPosition, 
			translatedPickRay, 
			*v1, 
			*v2, 
			*v3, 
			contact, 
			length)
		) {

			// can be modified later to save individual polygons, but for now
			// save only if picked in front of ray
			if (length < pickedObject.distance && length > 0) {

				pickedObject.hasBeenPicked = true;
				pickedObject.distance = length;
			}
		}
	}

	return pickedObject;
}

PickObject Camera::GetPickedObject(std::map<std::string, Object3D*>* objects, glm::vec3 pickingRay) {

	PickObject picked;
	for (auto const& objRef : *objects) {

		PickObject newPick = ObjectPicked(objRef.second, pickingRay);

		if (newPick.distance < picked.distance)
			picked = newPick;
	}

	return picked;
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
			glm::vec3 direction = camPosition - camTarget;

			// preform pitch rotation
			glm::vec3 cross = glm::normalize(glm::cross(direction, camUp));

			// Rotate Mat4 By Vec3
			cameraRotation = glm::rotate(cameraRotation, -diffY, glm::cross(direction, camUp));

			// preform yaw rotation
			cameraRotation = glm::rotate(cameraRotation, diffX, glm::vec3(0, 1, 0));

			// calculate new direction
			direction = glm::vec3(glm::vec4(direction, 0) * cameraRotation);

			// set new position
			if (arcballCam) {

				camPosition = (direction + camTarget);
			}
			else {

				camTarget = (camPosition - direction);
			}

			// calculate up
			camUp = camUp * glm::mat3(cameraRotation);
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
		glm::vec3 direction = camPosition - camTarget;

		// calculate change horisontally
		glm::vec3 horz = glm::normalize(glm::cross(direction, camUp));

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

	glm::vec3 direction = camPosition - camTarget;

	float focus = (float)(zoomDelta > 0 ? focusDelta : -focusDelta);

	if (glm::length(direction) > focusDelta || focus < 0) {

		direction = glm::normalize(direction);

		direction *= focus;

		camPosition -= direction;
	}
}

// move cameras position and target in space
void Camera::_MoveCam(glm::vec3 movement) {

	camPosition += movement;
	camTarget += movement;
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
			DEGS_TO_RADS(-objList[objIndex]->GetOrientationEuler().x),
			glm::vec3(1, 0, 0)
		);
		offsetMatrix = glm::rotate(
			offsetMatrix,
			DEGS_TO_RADS(-objList[objIndex]->GetOrientationEuler().y),
			glm::vec3(0, 1, 0)
		);
		offsetMatrix = glm::rotate(
			offsetMatrix,
			DEGS_TO_RADS(-objList[objIndex]->GetOrientationEuler().z),
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
	glm::vec4 direction = glm::vec4(cam.camTarget, 1);

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

void Camera::LookThrough(RenderingContext& rcontext) {

	// calcuate camera lookat atrix
	rcontext.viewmatrix = glm::lookAt(
		camPosition,
		camTarget,
		camUp
	);

	// translate matrix by entity position
	if (parent != nullptr) {

		TranslationStack camTranslation = GetWorldTranslation();

		rcontext.viewmatrix *= glm::inverse(*camTranslation.GetCurrentModelMatrix());
	}
}