#include "Animator.h"



Animator::Animator() {

}

Animator::Animator(std::map<std::string, Object3D*>* objectRefs) {

	objects = objectRefs;

	s_platter.part = Part::PLATTER;
	s_platter.object = (*objects)["Platter"];

	s_arm.part = Part::ARM;
	s_arm.object = (*objects)["Arm"];

	s_seats.part = Part::SEATS;
	s_seats.object = (*objects)["Spinner"];

	s_tilt.part = Part::TILT;
	s_tilt.object = (*objects)["Pivot"];

	e_smoke.part = Part::SMOKE;
	e_smoke.particles.push_back(dynamic_cast<Particle*>((*objects)["DryIce"]));
	e_smoke.particles.push_back(dynamic_cast<Particle*>((*objects)["DryIce2"]));

	e_fire.part = Part::FIRE;
	e_fire.particles.push_back(dynamic_cast<Particle*>((*objects)["Fire"]));
	e_fire.particles.push_back(dynamic_cast<Particle*>((*objects)["FireSmoke"]));
}


Animator::~Animator() {

}

// preform animation of the RideStates gievn a difference in time
void Animator::Animate(double step) {

	time_ms += (step * 1000);

	_AnimatePart(s_platter);
	_AnimatePart(s_arm);
	_AnimatePart(s_tilt);
	_AnimatePart(s_seats);

	_AnimateEffect(e_smoke);
	_AnimateEffect(e_fire);
}

// preform the animation change to a RideState according to its current state
void Animator::_AnimatePart(RideState& state) {

	// keep animating transition phase
	if (state.isBusy) {

		// has the animation finished
		if (state.whenFinished > time_ms) {

			// don't calculate aCurrent when waiting
			if (!state.isWaiting) {

				// get progress as 0-1 range
				float progress =
					(time_ms - state.whenStarted)
					/ (state.whenFinished - state.whenStarted);

				// get transition
				Speed* current = _CalculateAnimationState(
					state.aStrt,
					state.aEnd,
					progress
				);

				// assign as current
				state.aCurrent[0] = current[0];
				state.aCurrent[1] = current[1];
				state.aCurrent[2] = current[2];
			}

			// preform animation onto object
			// _AnimateObject(state);
		}
		else {

			// save the new animation state
			state.aCurrent[0] = state.aStrt[0] = state.aEnd[0];
			state.aCurrent[1] = state.aStrt[1] = state.aEnd[1];
			state.aCurrent[2] = state.aStrt[2] = state.aEnd[2];

			// free the animation & get next operation
			state.isBusy = false;
			state.isWaiting = false;
			state.queue.pop();
		}
	}
	
	// get the next animation
	else {

		// if there are actions to do
		if (state.queue.size() > 0) {

			// get next transition
			Change change = state.queue.front();

			switch (change) {

				// if waiting, no change in animation
				case Change::WAIT_1:
				case Change::WAIT_5:
				case Change::WAIT_10:
				case Change::WAIT_20: {

					state.aEnd[0] = state.aStrt[0];
					state.aEnd[1] = state.aStrt[1];
					state.aEnd[2] = state.aStrt[2];

					state.isWaiting = true;
					break;
				}

				// otherwise get the end state required
				default: {

					// get transition
					Speed* newEnd = _NewState(
						state.part,
						change
					);

					// assign as current
					state.aEnd[0] = newEnd[0];
					state.aEnd[1] = newEnd[1];
					state.aEnd[2] = newEnd[2];
					break;
				}

			}

			// assign length of time required for animation
			state.whenStarted = time_ms;
			state.whenFinished = time_ms + _Time(change);

			// set busy
			state.isBusy = true;
		}
	}

	// preform animation onto object
	_AnimateObject(state);
}

// preform the effect change to a RideState according to its current state
void Animator::_AnimateEffect(RideEffect& effect) {

	// keep animating transition phase
	if (effect.isBusy) {

		// when the effect has finished
		if (effect.whenFinished < time_ms) {

			// free the animation & get next operation
			effect.isBusy = false;
			effect.isWaiting = false;
			effect.queue.pop();
		}
	}

	// get the next effect
	else {

		// if there are actions to do
		if (effect.queue.size() > 0) {

			// get next transition
			Change change = effect.queue.front();

			switch (change) {

				// if waiting, no change in animation
				case Change::WAIT_1:
				case Change::WAIT_5:
				case Change::WAIT_10:
				case Change::WAIT_20:
					effect.isWaiting = true;
					break;

				// toggle the particle effects on
				case Change::ON:
					for (Particle* p : effect.particles)
						p->active = true;
					break;

				// toggle the particle effects off
				case Change::OFF:
				default:
					for (Particle* p : effect.particles)
						p->active = false;
					break;
			}

			// assign length of time required for animation
			effect.whenStarted = time_ms;
			effect.whenFinished = time_ms + _Time(change);

			// set busy
			effect.isBusy = true;
		}
	}
}

// do the rotational change to RideState's associated object
void Animator::_AnimateObject(RideState& state) {

	state.object->ShiftRotation(
		state.aCurrent[0] * speed,
		state.aCurrent[1] * speed,
		state.aCurrent[2] * speed
	);
}

// calculate the appropriate state between two speeds given the progress between their change
Speed* Animator::_CalculateAnimationState(Speed* start, Speed* end, float progress) {

	// convert progress value from 0-1 to PI-2PI
	float degrees = (progress * 180) + 180;
	float rads = degrees * (M_PI / 180);

	// diff is now shifted back to 0-1 range
	float diff = (cos(rads) / 2) + 0.5;

	float xDiff = end[0] - start[0];
	float yDiff = end[1] - start[1];
	float zDiff = end[2] - start[2];

	float newValue[3] = {
		start[0] + (xDiff * diff),
		start[1] + (yDiff * diff),
		start[2] + (zDiff * diff),
	};

	return newValue;
}

// return the state of a given Change according to the Part it is associated with
Speed* Animator::_NewState(Part p, Change c) {

	switch (p) {

		case Part::PLATTER: {

			switch (c) {

				case Change::ROT_C:		return _MakeState(0.0f, 1.0f, 0.0f);
				case Change::ROT_AC:	return _MakeState(0.0f, -1.0f, 0.0f);
				case Change::STEP_C:	return _MakeState(0.0f, 1.0f, 0.0f);
				case Change::STEP_AC:	return _MakeState(0.0f, -1.0f, 0.0f);
				case Change::STOP:		return _MakeState(0.0f, 0.0f, 0.0f);
			}
		}
		case Part::ARM: {

			switch (c) {

				case Change::ROT_C:		return _MakeState(0.0f, 0.0f, 1.0f);
				case Change::ROT_AC:	return _MakeState(0.0f, 0.0f, -1.0f);
				case Change::STEP_C:	return _MakeState(0.0f, 0.0f, 1.0f);
				case Change::STEP_AC:	return _MakeState(0.0f, 0.0f, -1.0f);
				case Change::STOP:		return _MakeState(0.0f, 0.0f, 0.0f);
			}
		}
		case Part::SEATS: {

			switch (c) {

				case Change::ROT_C:		return _MakeState(0.0f, 1.0f, 0.0f);
				case Change::ROT_AC:	return _MakeState(0.0f, -1.0f, 0.0f);
				case Change::STEP_C:	return _MakeState(0.0f, 1.0f, 0.0f);
				case Change::STEP_AC:	return _MakeState(0.0f, -1.0f, 0.0f);
				case Change::STOP:		return _MakeState(0.0f, 0.0f, 0.0f);
			}
		}
		case Part::TILT: {

			switch (c) {

				case Change::TILT_UP:	return _MakeState(-0.05f, 0.0f, 0.0f);
				case Change::TILT_DOWN:	return _MakeState(0.05f, 0.0f, 0.0f);
				case Change::STOP:		return _MakeState(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// all defaults back to stopping
	return _MakeState(0.0f, 0.0f, 0.0f);
}

// helper function to stitch 3 floats into a Speed[3] array
Speed* Animator::_MakeState(float x, float y, float z) {

	Speed state[3] = { x, y, z };
	return state;
}

// route Changes to the appropriate RideState
void Animator::_Do(Part part, Change change) {

	switch (part) {

	case Part::PLATTER:
		s_platter.queue.push(change);
		break;

	case Part::ARM:
		s_arm.queue.push(change);
		break;

	case Part::SEATS:
		s_seats.queue.push(change);
		break;

	case Part::TILT:
		s_tilt.queue.push(change);
		break;

	case Part::ALL:
		s_platter.queue.push(change);
		s_arm.queue.push(change);
		s_seats.queue.push(change);
		s_tilt.queue.push(change);
		break;

	case Part::SMOKE:
		e_smoke.queue.push(change);
		break;

	case Part::FIRE:
		e_fire.queue.push(change);
		break;
	}
}

// return the animation time for a given change
long Animator::_Time(Change rc) {

	switch (rc) {

	case Change::WAIT_10:
		return 10000;

	case Change::WAIT_20:
		return 20000;

	case Change::WAIT_5:
		return 5000;

	case Change::WAIT_1:
		return 1000;

	case Change::TILT_UP:
	case Change::TILT_DOWN:
		return 1000;

	case Change::ON:
	case Change::OFF:
		return 0;

	default:
		return 5000;
	}
}