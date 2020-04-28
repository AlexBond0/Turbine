#include "MoveableTarget.h"


MoveableTarget::MoveableTarget() {

	_target = glm::vec3(0);
}

MoveableTarget::MoveableTarget(MoveableTarget* copy) {

	_target = copy->_target;
}

// =========================================================

void MoveableTarget::SetTarget(glm::vec3 target) {

	_target = target;
}

glm::vec3* MoveableTarget::GetTarget() {

	return &_target;
}