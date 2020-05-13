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

	void UpdateInstanceCount();
	void UpdateInstanceCount(int instanceCount);
	int GetInstanceCount();
	int InstanceDataSize();

	virtual json Serialize();
	virtual void Deserialize(json& data);

	InstanceData instances;

	bool serializeInstanceData = true;

protected:

	int _noofinstances = 0;	// number of instances to render

	void _InitVBOs();
	void _Draw(RenderingContext& rcontext);
	void _HandleVBOs(RenderingContext& rcontext);
};