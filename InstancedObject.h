#pragma once
#include "Object3D.h"
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