#include "Moveable.h"

Moveable::Moveable() {

}

Moveable::Moveable(Moveable* copy)
	: MoveablePoint(MoveablePoint(copy))
	, MoveableOrientation(MoveableOrientation(copy))
{

}

// point towards a vector from a given starting vector direction
void Moveable::PointAt(glm::vec3 newVector) {

	newVector = glm::normalize(newVector);
	_orientation = _RotationBetweenVectors(_up, newVector);
}


void Moveable::LookAt(glm::vec3 lookAt) {

	glm::vec3 direction = lookAt - _localPos;

	direction = glm::normalize(direction);
	_orientation = _RotationBetweenVectors(_up, direction);

	//glm::quat rot1 = _RotationBetweenVectors(_front, direction);

	//// Recompute desiredUp so that it's perpendicular to the direction
	//// You can skip that part if you really want to force desiredUp
	//glm::vec3 right = cross(direction, _up);
	//_up = cross(right, direction);

	//// Because of the 1rst rotation, the up is probably completely screwed up.
	//// Find the rotation between the "up" of the rotated object, and the desired up
	//glm::vec3 newUp = rot1 * glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::quat rot2 = _RotationBetweenVectors(newUp, _up);

	//_orientation = rot2 * rot1; // remember, in reverse order.
}


void Moveable::LookAtTarget() {

	if (useTarget)
		LookAt(_target);
}


void Moveable::Move(glm::vec3 direction) {

	_localPos += direction;
	_target += direction;
}

// Returns a quaternion such that q*start = dest
glm::quat Moveable::_RotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {

		// special case when vectors in opposite directions :
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		// This implementation favors a rotation around the Up axis,
		// since it's often what you want to do.
		rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return angleAxis(glm::radians(180.0f), rotationAxis);
	}

	// Implementation from Stan Melax's Game Programming Gems 1 article
	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

json Moveable::Serialize() {

	json mp;

	Serialize::SerializeVec3(mp, "_worldPos", _worldPos);
	Serialize::SerializeVec3(mp, "_localPos", _localPos);
	Serialize::SerializeVec3(mp, "_translation", _translation);
	Serialize::SerializeVec3(mp, "_scale", _scale);
	Serialize::SerializeVec3(mp, "_up", _up);

	json mo;
	mo["useTarget"] = useTarget;
	Serialize::SerializeVec3(mo, "_target", _target);
	Serialize::SerializeVec3(mo, "_front", _front);
	Serialize::SerializeQuat(mo, "_orientation", _orientation);
	Serialize::SerializeQuat(mo, "_rotation", _rotation);

	json me;
	me["MoveablePoint"] = mp;
	me["MoveableOrientation"] = mo;

	// pack and send json
	json ret;
	ret["Moveable"] = me;
	return ret;
}

Moveable::Moveable(json& data) {

	_worldPos		= Serialize::DeserializeVec3(data["MoveablePoint"]["_worldPos"]);
	_localPos		= Serialize::DeserializeVec3(data["MoveablePoint"]["_localPos"]);
	_translation	= Serialize::DeserializeVec3(data["MoveablePoint"]["_translation"]);
	_scale			= Serialize::DeserializeVec3(data["MoveablePoint"]["_scale"]);
	_up				= Serialize::DeserializeVec3(data["MoveablePoint"]["_up"]);

	_target			= Serialize::DeserializeVec3(data["MoveableOrientation"]["_target"]);
	_front			= Serialize::DeserializeVec3(data["MoveableOrientation"]["_front"]);
	_orientation	= Serialize::DeserializeQuat(data["MoveableOrientation"]["_orientation"]);
	_rotation		= Serialize::DeserializeQuat(data["MoveableOrientation"]["_rotation"]);
}