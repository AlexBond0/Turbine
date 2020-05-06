#pragma once

#include "Matrix.h"
#include <Windows.h>
#include <vector>
#include "GLSetup.h"


// struct defining layout of a polygon
struct Instance {

	glm::vec3 position;
};

class InstanceData
{
public:
	InstanceData() {};
	~InstanceData() {};

	void SetData(InstanceData pointData);
	void AddInstance(Instance newInstance);

	std::vector<Instance>* GetVector();
	void* GetData();

	int Size();
	int ElementCount();
	int DataSize();
	void Reserve(int newPolys);

private:

	std::vector<Instance> _instance_struct_data;	// Storage of Poly elements defining the object

	void _SetData(std::vector<Instance> instances);
};

