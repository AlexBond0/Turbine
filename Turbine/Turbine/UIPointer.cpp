#include "UIPointer.h"



UIPointer::UIPointer(Entity* entityToMirror) {


	_uiObjs["PointerBall"] = new Primitive();
	_uiObjs["PointerBall"]->GenerateDirector(0.005f);
	_uiObjs["PointerBall"]->SetName(entityToMirror->GetName()+" UI Pointer");

	_uiObjs["PointerTarget"] = new Primitive();
	Geomitory sphere = Primitive::GenerateBaseIcoSphere(0.005f);
	_uiObjs["PointerTarget"]->AssignGeomitoryData(sphere);
	_uiObjs["PointerTarget"]->SetName(entityToMirror->GetName() + " UI Target");
	_uiObjs["PointerTarget"]->useLight = false;

	_uiObjs["BasicBall"] = new Primitive();
	_uiObjs["BasicBall"]->AssignGeomitoryData(sphere);
	_uiObjs["BasicBall"]->SetName(entityToMirror->GetName() + " UI Ball");
	_uiObjs["BasicBall"]->useLight = false;

	_entityToMirror = entityToMirror;
}


UIPointer::~UIPointer() {

	for (auto const& uiObj : _uiObjs)
		delete uiObj.second;
}

void UIPointer::RenderUI(RenderingContext& rcontext) {

	if (showUIObject) {

		if (showDirection) {

			// position
			if (_usePosition)
				_uiObjs["PointerBall"]->SetLocalPos(*_position);

			else
				_uiObjs["PointerBall"]->SetLocalPos(*_entityToMirror->GetLocalPosVec());


			// orientation
			if (_target) {

				_uiObjs["PointerBall"]->LookAt(*_target);

				_uiObjs["PointerTarget"]->SetLocalPos(*_target);
				_uiObjs["PointerTarget"]->Draw(rcontext);
			}

			else
				_uiObjs["PointerBall"]->SetOrientation(_entityToMirror->GetOrientationQuat());

			_uiObjs["PointerBall"]->Draw(rcontext);
		}
		else {

			// position
			if (_usePosition)
				_uiObjs["BasicBall"]->SetLocalPos(*_position);

			else
				_uiObjs["BasicBall"]->SetLocalPos(*_entityToMirror->GetLocalPosVec());

			_uiObjs["BasicBall"]->Draw(rcontext);
		}
	}
}

Object3D* UIPointer::GetObject3D() {

	return _uiObjs["PointerBall"];
}

void UIPointer::UsePosition(glm::vec3* position) {

	_usePosition = true;
	_position = position;
}

void UIPointer::UseTarget(glm::vec3* target) {

	_useTarget = true;
	_target = target;
}

void UIPointer::SetColor(color4 color) {

	_uiObjs["PointerBall"]->SetAmbient(color);
	_uiObjs["PointerTarget"]->SetAmbient(color);
	_uiObjs["BasicBall"]->SetAmbient(color);
}

void UIPointer::_GenXYZBall(Entity* entityToMirror) {

	float radius = 0.005;
	_uiObjs["PointerBall"] = new Primitive();
	_uiObjs["PointerBall"]->SetName(entityToMirror->GetName() + " UI Pointer");

	// create bvase sphere
	Geomitory sphere = Primitive::GenerateBaseIcoSphere(radius);
	_uiObjs["PointerBall"]->AssignGeomitoryData(sphere);
	_uiObjs["PointerBall"]->useLight = false;
	_uiObjs["PointerBall"]->SetAmbient(color4(1.0, 1.0, 1.0, 1.0));

	// create pole
	Geomitory pole = Primitive::GenerateCylinder((radius / 3), (radius * 2), 4);
	Primitive::TranslateGeomitory(pole, 0.0f, radius, 0.0f);

	// x direction
	_uiObjs["PointerX"] = new Primitive();
	_uiObjs["PointerX"]->SetName(_uiObjs["PointerBall"]->GetName() + " - X");
	_uiObjs["PointerX"]->AssignGeomitoryData(pole);
	_uiObjs["PointerX"]->SetOrientation(0.0f, 0.0f, 90.0f);
	_uiObjs["PointerX"]->useLight = false;
	_uiObjs["PointerX"]->SetAmbient(color4(1.0, 0.0, 0.0, 1.0));
	_uiObjs["PointerBall"]->AddChild(_uiObjs["PointerX"]);

	// x direction
	_uiObjs["PointerY"] = new Primitive();
	_uiObjs["PointerY"]->SetName(_uiObjs["PointerBall"]->GetName() + " - Y");
	_uiObjs["PointerY"]->AssignGeomitoryData(pole);
	_uiObjs["PointerY"]->SetOrientation(0.0f, 0.0f, 0.0f);
	_uiObjs["PointerY"]->useLight = false;
	_uiObjs["PointerY"]->SetAmbient(color4(0.0, 1.0, 0.0, 1.0));
	_uiObjs["PointerBall"]->AddChild(_uiObjs["PointerY"]);

	// x direction
	_uiObjs["PointerZ"] = new Primitive();
	_uiObjs["PointerZ"]->SetName(_uiObjs["PointerBall"]->GetName() + " - Z");
	_uiObjs["PointerZ"]->AssignGeomitoryData(pole);
	_uiObjs["PointerZ"]->SetOrientation(90.0f, 0.0f, 0.0f);
	_uiObjs["PointerZ"]->useLight = false;
	_uiObjs["PointerZ"]->SetAmbient(color4(0.0, 0.0, 1.0, 1.0));
	_uiObjs["PointerBall"]->AddChild(_uiObjs["PointerZ"]);
}