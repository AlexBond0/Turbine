#pragma once

#include "Animator.h"

class RideAnimation :
	public Animator
{
public:

	RideAnimation();
	RideAnimation(std::map<std::string, Object3D*>* objectRefs);
	~RideAnimation();

	void QueueSpinStart();
	void QueueSpinEnd();

	void QueueStandardRide();
	void QueueLongRide();

	void MovePart(Part part, Change change);

	void ResetRotations();

private:

	float TILT_MIN = 0.0f;
	float TILT_MAX = 25.0f;

	void _MoveTilt(Change change);
};

inline RideAnimation::RideAnimation()
	: Animator()
{

};

inline RideAnimation::RideAnimation(std::map<std::string, Object3D*>* objectRefs)
	: Animator(objectRefs) 
{

};

inline RideAnimation::~RideAnimation() {

};