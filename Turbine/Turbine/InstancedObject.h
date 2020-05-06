#pragma once

#include "Object3D.h"
#include "InstanceData.h"


// An Object3D but using instancing to replicate many of the same Object3D data
// efficiently on the GPU
class InstancedObject 
	: public Object3D
{
public:
	InstancedObject(std::string name);
	InstancedObject(Object3D* copy, std::string newName);
	virtual ~InstancedObject();

	void SetInstanceData(int instanceCount);
	int GetInstanceCount();
	int InstanceDataSize();

	InstanceData instances;

protected:

	// instance data
	// float* instanceData;
	int _noofinstances = 0;


	void _InitVBOs();

	void _Draw(RenderingContext& rcontext);

	void _HandleVBOs(RenderingContext& rcontext);
};