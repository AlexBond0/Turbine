
#include "Camera.h"


Camera::Camera(std::string name)
	: Entity(name, EntityType::CAMERA),
	cameraUI(this)
{

	DefaultSceneCam();

	useTarget = true;

	cameraUI.UsePosition(&_localPos);
	cameraUI.UseTarget(&_target);
}


Camera::~Camera() {

}

// Handle action taken by Object3D when called form entity heirarcy
void Camera::OnRender(RenderingContext& rcontext) {

	cameraUI.RenderUI(rcontext);
}

// Handle action when sending data to the picking function
Object3D* Camera::OnPick() {

	return cameraUI.GetObject3D();
}


// ================================================================

void Camera::DefaultSceneCam() {

	SetTarget(glm::vec3(0.0f, 0.2f, 0.0f));
	SetLocalPos(glm::vec3(-0.8f, 0.5f, 1.0f));
	SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::DefaultPOVCam() {

	// these are more like offsets here as the camerapositioner handles the absolute position
	SetTarget(glm::vec3(0.0f, 0.0f, 1.0f));
	SetLocalPos(glm::vec3(0.0f, 0.0f, 0.0f));
	SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

// given a mouse xy and view xy, calculate a direction vertex for a picking ray
glm::vec3 Camera::CalculatePickRay(float mouseX, float mouseY, float windW, float windH) {

	// positions need to be between -1 and 1
	float clickPosX = mouseX / (windW  * 0.5f) - 1.0f;
	float clickPosY = mouseY / (windH * 0.5f) - 1.0f;

	// get diection of camera
	glm::vec3 direction = _localPos - _target;

	// calculate projection and view matrix for the camera
	glm::mat4 proj = glm::perspective(fFovy, fAspect, fZNear, fZFar);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f), -direction, _up);

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
		* glm::vec4(_localPos, 1.0);

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

PickObject Camera::GetPickedObject(EntityManager* entities, glm::vec3 pickingRay) {

	PickObject picked;
	Object3D* objRef;

	for (auto const& entityRef : entities->GetAllEntities()) {

		// models need checking recursivey
		// could be swapped for bounding box thing
		if (entityRef.second->GetEntityType() == EntityType::MODEL) {

			Model* model = dynamic_cast<Model*>(entityRef.second);
			PickObject newPick = GetPickedObject(model, pickingRay);

			if (newPick.distance < picked.distance)
				picked = newPick;
		}

		objRef = entityRef.second->OnPick();

		// does entity have pickable geometory
		if (objRef != nullptr) {

			// is the geometory active
			if (objRef->isLocallyActive && objRef->IsGloballyActive()) {

				PickObject newPick = ObjectPicked(objRef, pickingRay);

				if (newPick.distance < picked.distance)
					picked = newPick;
			}
		}
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
			glm::vec3 direction = _localPos - _target;

			// preform pitch rotation
			glm::vec3 cross = glm::normalize(glm::cross(direction, _up));

			// Rotate Mat4 By Vec3
			cameraRotation = glm::rotate(cameraRotation, -diffY, glm::cross(direction, _up));

			// preform yaw rotation
			cameraRotation = glm::rotate(cameraRotation, diffX, glm::vec3(0, 1, 0));

			// calculate new direction
			direction = glm::vec3(glm::vec4(direction, 0) * cameraRotation);

			// set new position
			if (arcballCam) {

				_localPos = (direction + _target);
			}
			else {

				_target = (_localPos - direction);
			}

			// calculate up
			_up = _up * glm::mat3(cameraRotation);
		}

		// save previous values
		_prevX = newX;
		_prevY = newY;
		_prevValuesDirty = false;
	}
}

// move camrea by a set value for x and y
void Camera::MoveCam(int newX, int newY) {

	if (moveable) {

		// account for mouse moving when not dragging
		if (!_prevValuesDirty) {

			// get difference
			float diffX = float(newX - _prevX) / 50.0f;
			float diffY = float(newY - _prevY) / 50.0f;

			// get the direction vector
			glm::vec3 direction = _localPos - _target;

			// calculate change horisontally
			glm::vec3 horz = glm::normalize(glm::cross(direction, _up));

			// calculate change vertically
			glm::vec3 vert = glm::normalize(glm::cross(horz, direction));

			// scale the changes
			horz *= diffX;
			vert *= diffY;

			// apply the changes
			Move(horz + vert);
		}

		// save previous values
		_prevX = newX;
		_prevY = newY;
		_prevValuesDirty = false;
	}
}

// Move the camera in a dolly action (forward & backward)
void Camera::MoveDolly(float ammount) {

	if (moveable) {

		glm::vec3 direction = glm::normalize(GetWorldPosition() - _target);

		Move(direction * -ammount);
	}
}

// Move the camera in a truck action (left & right)
void Camera::MoveTruck(float ammount) {

	if (moveable) {

		glm::vec3 direction = glm::normalize(GetWorldPosition() - _target);
		glm::vec3 horz = glm::normalize(glm::cross(direction, _up));

		Move(horz * ammount);
	}
}

void Camera::PerformDeltaMovements() {

	if (movementDelta.truck != 0)
		MoveTruck(movementDelta.truck);

	if (movementDelta.dolly != 0)
		MoveDolly(movementDelta.dolly);
}

void Camera::FinishMovement() {

	_prevValuesDirty = true;
}


// change relationship between position & target
void Camera::FocusCam(int zoomDelta) {

	if (moveable) {

		glm::vec3 direction = _localPos - _target;

		float focus = (float)(zoomDelta > 0 ? focusDelta : -focusDelta);

		if (glm::length(direction) > focusDelta || focus < 0) {

			direction = glm::normalize(direction);

			direction *= focus;

			_localPos -= direction;
		}
	}
}

// Produce a viewmatrix in rcontext for the camera
void Camera::LookThrough(RenderingContext& rcontext) {

	// handle delta camera movements
	PerformDeltaMovements();

	// Calculate camera projection matrix if values changed
	if (fDirty) {

 		CalculateProjection(rcontext);
		fDirty = false;
	}

	// calcuate camera lookat atrix
	rcontext.viewMatrix = glm::lookAt(
		_localPos,
		_target,
		_up
	);

	// translate matrix by entity position
	if (parent != nullptr) {

		TranslationStack camTranslation = parent->GetWorldTranslation();

		rcontext.viewMatrix *= glm::inverse(*camTranslation.GetCurrentModelMatrix());
	}
}

// Calculate the projection matrix from the camera's state and the new window dimentions
void Camera::CalculateProjection(RenderingContext& rcontext, int width, int height) {

	fAspect = (float)width / height;

	CalculateProjection(rcontext);
}

// Calculate the projection matrix from the camera's state
void Camera::CalculateProjection(RenderingContext& rcontext) {

	float top = (float)(tan(fFovy * 0.5) * fZNear);

	float bottom = -top;
	float left = fAspect * bottom;
	float right = fAspect * top;

	rcontext.projectionMatrix = glm::frustum(
		left,
		right,
		bottom,
		top,
		fZNear,
		fZFar
	);
}

// Calculate the aspect ratio for the camera and flag for recalculation of projection matrix
void Camera::UpdateAspectRatio(int width, int height) {

	fAspect = (float)width / height;
	fDirty = true;
}


json Camera::Serialize() {

	json me = Entity::Serialize();

	me["fZNear"] = fZNear;
	me["fZFar"] = fZFar;
	me["fAspect"] = fAspect;

	me["focusDelta"] = focusDelta;

	// pack and send json
	json ret;
	ret["Camera"] = me;
	return ret;
}

Camera::Camera(json& data)
	: Entity(data["Entity"])
	, cameraUI(this) {

	fZNear	= data["fZNear"];
	fZFar	= data["fZFar"];
	fAspect = data["fAspect"];

	focusDelta = data["focusDelta"];

	useTarget = true;

	cameraUI.UsePosition(&_localPos);
	cameraUI.UseTarget(&_target);
}