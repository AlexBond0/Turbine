#pragma once
#include "Object3D.h"


// An Object3D but using instancing to replicate many of the same Object3D data
// efficiently on the GPU
class InstancedObject 
	: public Object3D
{
public:
	InstancedObject();
	InstancedObject(Object3D* copy, std::string newName);
	virtual ~InstancedObject();

	void SetInstanceData(float* newInstanceData, int noofnewinstances);
	int GetInstanceCount();

protected:

	// instance data
	float* instanceData;
	int noofinstances;

	void _InitVBOs();

	void _HandleVBOs(RenderingContext& rcontext);
};


inline int InstancedObject::GetInstanceCount() {

	return noofinstances;
}