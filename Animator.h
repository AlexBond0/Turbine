#pragma once

#include "Object3D.h"
#include "Particle.h"
#include <queue>
#include <tuple>

// which part is being animated
enum class Part {
	PLATTER,	// the rides platter component
	ARM,		// the rides arm component
	SEATS,		// the rides seats component
	TILT,		// the rides tilt component
	ALL,		// all of the animatable components
	SMOKE,		// the rides dry ice effect
	FIRE		// the rides fire effect
};

// what animation to complete
enum class Change {
	ROT_C,		// rotate the part clockwise
	ROT_AC,		// rotate the part anti-clockwise
	STEP_C,		// step the part clockwise by 90 degrees
	STEP_AC,	// step the part anti-clockwise by 90 degrees
	STOP,		// stop the part moving
	TILT_UP,	// tilt the part up
	TILT_DOWN,	// tilt the part down

	WAIT_1,		// wait 1 second till the next operation
	WAIT_5,		// wait 5 seconds till the next operation
	WAIT_10,	// wait 10 seconds till the next operation
	WAIT_20,	// wait 20 seconds till the next operation

	ON,			// toggle effect on
	OFF			// toggle effect off
};

// to distinguish an object's change of rotation from regular rotation
typedef float Speed;

// state information about a parts animation
struct RideState {

	Part part;
	std::queue<Change> queue;

	bool isBusy = false;		// is there an animation currently playing?
	bool isWaiting = false;		// is the current animation a waiting time?
	float whenStarted = 0.0f;	// start animation time
	float whenFinished = 0.0f;	// end animation time

	Speed aStrt[3] = { 0.0f, 0.0f, 0.0f };
	Speed aCurrent[3] = { 0.0f, 0.0f, 0.0f };
	Speed aEnd[3] = { 0.0f, 0.0f, 0.0f };

	Object3D* object = nullptr;
};

// state information about an effects animation
struct RideEffect {

	Part part;
	std::queue<Change> queue;

	bool isBusy = false;		// is there an animation currently playing?
	bool isWaiting = false;		// is the current animation a waiting time?
	float whenStarted = 0.0f;	// start animation time
	float whenFinished = 0.0f;	// end animation time

	std::vector<Particle*> particles;
};

class Animator {

public:

	Animator();
	Animator(std::map<std::string, Object3D*>* objectRefs);
	~Animator();

	void Animate(long step);

protected:

	long time_ms = 0;	// current time of animation
	long speed = 3;		// speed (adjust for smaller time spacing)

	std::map<std::string, Object3D*>* objects;	// the map of all object pointers

	RideState s_platter;	// the platter RideState
	RideState s_arm;		// the arm RideState
	RideState s_seats;		// the seats RideState
	RideState s_tilt;		// the tilt RideState

	RideEffect e_smoke;		// the smoke RideEffect
	RideEffect e_fire;		// the fire RideEffect

	void _Do(Part part, Change change);

	Speed* _NewState(Part p, Change c);
	long _Time(Change rc);

private:

	void _AnimatePart(RideState& state);
	void _AnimateEffect(RideEffect& effect);
	
	void _AnimateObject(RideState& state);

	Speed* _CalculateAnimationState(Speed* start, Speed* end, float progress);

	Speed* _MakeState(float x, float y, float z);
};

