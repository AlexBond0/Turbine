#include "RideAnimation.h"

// queue the simple spin start animation
void RideAnimation::QueueSpinStart() {

	_Do(Part::PLATTER, Change::ROT_C);
	_Do(Part::TILT, Change::TILT_UP);
	_Do(Part::TILT, Change::STOP);
	_Do(Part::SEATS, Change::WAIT_5);
	_Do(Part::SEATS, Change::ROT_C);
}

// queue the simple spin stop animation
void RideAnimation::QueueSpinEnd() {

	_Do(Part::SEATS, Change::STOP);
	_Do(Part::TILT, Change::TILT_DOWN);
	_Do(Part::TILT, Change::STOP);
	_Do(Part::PLATTER, Change::WAIT_5);
	_Do(Part::PLATTER, Change::STOP);
}

// queue a standard ride into the animation queues
void RideAnimation::QueueStandardRide() {

	// ===================================
	// platter animation : 50
	_Do(Part::PLATTER, Change::ROT_C);
	_Do(Part::PLATTER, Change::WAIT_20);
	_Do(Part::PLATTER, Change::WAIT_20);
	_Do(Part::PLATTER, Change::STOP);

	// ===================================
	// tilt animation : 42
	_Do(Part::TILT, Change::WAIT_5);
	_Do(Part::TILT, Change::TILT_UP);
	_Do(Part::TILT, Change::STOP);
	_Do(Part::TILT, Change::WAIT_20);
	_Do(Part::TILT, Change::WAIT_10);
	_Do(Part::TILT, Change::WAIT_5);
	_Do(Part::TILT, Change::TILT_DOWN);
	_Do(Part::TILT, Change::STOP);

	// ===================================
	// seats animation : 45
	_Do(Part::SEATS, Change::WAIT_10);
	_Do(Part::SEATS, Change::ROT_AC);
	_Do(Part::SEATS, Change::ROT_C);
	_Do(Part::SEATS, Change::WAIT_10);
	_Do(Part::SEATS, Change::ROT_AC);
	_Do(Part::SEATS, Change::ROT_C);
	_Do(Part::SEATS, Change::STOP);

	// ===================================
	// arm animation : 50
	_Do(Part::ARM, Change::WAIT_10);
	_Do(Part::ARM, Change::ROT_AC);
	_Do(Part::ARM, Change::STOP);
	_Do(Part::ARM, Change::ROT_C);
	_Do(Part::ARM, Change::STOP);
	_Do(Part::ARM, Change::ROT_AC);
	_Do(Part::ARM, Change::STOP);
	_Do(Part::ARM, Change::ROT_C);
	_Do(Part::ARM, Change::STOP);

	// ===================================
	// smoke effect
	_Do(Part::SMOKE, Change::WAIT_10);
	_Do(Part::SMOKE, Change::ON);
	_Do(Part::SMOKE, Change::WAIT_5);
	_Do(Part::SMOKE, Change::OFF);
	_Do(Part::SMOKE, Change::WAIT_10);
	_Do(Part::SMOKE, Change::ON);
	_Do(Part::SMOKE, Change::WAIT_5);
	_Do(Part::SMOKE, Change::OFF);

	// ===================================
	// fire effect
	_Do(Part::FIRE, Change::WAIT_10);
	_Do(Part::FIRE, Change::WAIT_5);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::ON);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::OFF);

	_Do(Part::FIRE, Change::WAIT_10);
	_Do(Part::FIRE, Change::WAIT_5);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::ON);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::OFF);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::ON);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::OFF);
}

// queue a long ride into the animation queues
void RideAnimation::QueueLongRide() {


	// ===================================
	// platter animation : 75
	_Do(Part::PLATTER, Change::ROT_C);
	_Do(Part::PLATTER, Change::WAIT_20);
	_Do(Part::PLATTER, Change::WAIT_10);
	_Do(Part::PLATTER, Change::ROT_AC);
	_Do(Part::PLATTER, Change::WAIT_10);
	_Do(Part::PLATTER, Change::WAIT_20);
	_Do(Part::PLATTER, Change::STOP);

	// ===================================
	// tilt animation : 79
	_Do(Part::TILT, Change::WAIT_5);
	_Do(Part::TILT, Change::TILT_UP);
	_Do(Part::TILT, Change::STOP);
	_Do(Part::TILT, Change::WAIT_20);
	_Do(Part::TILT, Change::TILT_DOWN);
	_Do(Part::TILT, Change::STOP);
	_Do(Part::TILT, Change::TILT_UP);
	_Do(Part::TILT, Change::WAIT_5);
	_Do(Part::TILT, Change::STOP);
	_Do(Part::TILT, Change::WAIT_20);
	_Do(Part::TILT, Change::TILT_DOWN);
	_Do(Part::TILT, Change::WAIT_5);
	_Do(Part::TILT, Change::STOP);

	// ===================================
	// seats animation : 70
	_Do(Part::SEATS, Change::WAIT_10);
	_Do(Part::SEATS, Change::ROT_AC);
	_Do(Part::SEATS, Change::ROT_C);
	_Do(Part::SEATS, Change::WAIT_10);
	_Do(Part::SEATS, Change::ROT_AC);
	_Do(Part::SEATS, Change::ROT_C);
	_Do(Part::SEATS, Change::ROT_AC);
	_Do(Part::SEATS, Change::WAIT_20);
	_Do(Part::SEATS, Change::ROT_C);
	_Do(Part::SEATS, Change::STOP);

	// ===================================
	// arm animation : 65
	_Do(Part::ARM, Change::WAIT_10);
	_Do(Part::ARM, Change::ROT_AC);
	_Do(Part::ARM, Change::STOP);
	_Do(Part::ARM, Change::ROT_C);
	_Do(Part::ARM, Change::STOP);
	_Do(Part::ARM, Change::WAIT_10);
	_Do(Part::ARM, Change::WAIT_5);
	_Do(Part::ARM, Change::ROT_AC);
	_Do(Part::ARM, Change::STOP);
	_Do(Part::ARM, Change::ROT_C);
	_Do(Part::ARM, Change::STOP);

	// ===================================
	// smoke effect
	_Do(Part::SMOKE, Change::WAIT_10);
	_Do(Part::SMOKE, Change::ON);
	_Do(Part::SMOKE, Change::WAIT_5);
	_Do(Part::SMOKE, Change::OFF);
	_Do(Part::SMOKE, Change::WAIT_10);
	_Do(Part::SMOKE, Change::ON);
	_Do(Part::SMOKE, Change::WAIT_5);
	_Do(Part::SMOKE, Change::OFF);

	_Do(Part::SMOKE, Change::WAIT_5);
	_Do(Part::SMOKE, Change::ON);
	_Do(Part::SMOKE, Change::WAIT_1);
	_Do(Part::SMOKE, Change::OFF);
	_Do(Part::SMOKE, Change::WAIT_1);
	_Do(Part::SMOKE, Change::ON);
	_Do(Part::SMOKE, Change::WAIT_1);
	_Do(Part::SMOKE, Change::OFF);
	_Do(Part::SMOKE, Change::WAIT_1);
	_Do(Part::SMOKE, Change::ON);
	_Do(Part::SMOKE, Change::WAIT_1);
	_Do(Part::SMOKE, Change::OFF);

	_Do(Part::SMOKE, Change::WAIT_20);
	_Do(Part::SMOKE, Change::WAIT_5);
	_Do(Part::SMOKE, Change::WAIT_1);
	_Do(Part::SMOKE, Change::WAIT_1);
	_Do(Part::SMOKE, Change::ON);
	_Do(Part::SMOKE, Change::WAIT_5);
	_Do(Part::SMOKE, Change::OFF);


	// ===================================
	// fire effect
	_Do(Part::FIRE, Change::WAIT_10);
	_Do(Part::FIRE, Change::WAIT_5);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::ON);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::OFF);

	_Do(Part::FIRE, Change::WAIT_20);
	_Do(Part::FIRE, Change::WAIT_10);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::ON);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::WAIT_1);
	_Do(Part::FIRE, Change::OFF);

}

// move a single part according to a given change
void RideAnimation::MovePart(Part part, Change change) {

	switch (part) {

	case Part::PLATTER:
		s_platter.object->ShiftRotation(_NewState(part, change));
		break;

	case Part::ARM:
		s_arm.object->ShiftRotation(_NewState(part, change));
		break;

	case Part::SEATS:
		s_seats.object->ShiftRotation(_NewState(part, change));
		break;

	case Part::TILT:
		_MoveTilt(change);
		break;
	}
}

// set all rotations to 0
void RideAnimation::ResetRotations() {

	float noRotation[3] = { 0.0f, 0.0f, 0.0f };

	s_platter.object->SetRotation(noRotation);
	s_arm.object->SetRotation(noRotation);
	s_seats.object->SetRotation(noRotation);
	s_tilt.object->SetRotation(noRotation);
}

// move the tilt with respect to limits
void RideAnimation::_MoveTilt(Change change) {

	float currentTilt = -s_tilt.object->GetRotationVec().x;

	bool up = (change == Change::TILT_UP);

	float* scaledState = _NewState(Part::TILT, change);
	MyMatrix::Scale(scaledState, 6.0f);

	if (up && currentTilt < TILT_MAX)
		s_tilt.object->ShiftRotation(scaledState);
	
	else if (!up && currentTilt > TILT_MIN)
		s_tilt.object->ShiftRotation(scaledState);

}