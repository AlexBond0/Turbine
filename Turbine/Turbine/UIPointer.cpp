#include "UIPointer.h"



UIPointer::UIPointer(Entity* entityToMirror) {

	_uiObj = new Primitive();
	_uiObj->GenerateDirector(0.005f);

	_entityToMirror = entityToMirror;
}


UIPointer::~UIPointer() {

	if (_uiObj != nullptr)
		delete _uiObj;
}

void UIPointer::RenderUI(RenderingContext& rcontext) {

	if (showUIObject) {

		// position
		if (_usePosition)
			_uiObj->SetTranslation(*_position);

		else
			_uiObj->SetLocalPos(_entityToMirror->GetLocalPosVec());


		// orientation
		if (_target)
			_uiObj->PointAt(*_target);

		else
			_uiObj->SetOrientation(_entityToMirror->GetOrientationMatrix());


		_uiObj->Draw(rcontext);
	}
}

Object3D* UIPointer::GetObject3D() {

	return _uiObj;
}

void UIPointer::UsePosition(glm::vec3* position) {

	_usePosition = true;
	_position = position;
}

void UIPointer::UseTarget(glm::vec3* target) {

	_useTarget = true;
	_target = target;
}