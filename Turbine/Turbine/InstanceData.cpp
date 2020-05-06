#include "InstanceData.h"


// Assign polygon data from an existing PolygonData object
void InstanceData::SetData(InstanceData polygonData) {

	_instance_struct_data = polygonData._instance_struct_data;
}

// Add a new polygon to the Object
void InstanceData::AddInstance(Instance newInstance) {

	_instance_struct_data.push_back(newInstance);
}


// Assign polygon data to the Object
void InstanceData::_SetData(std::vector<Instance> instances) {

	_instance_struct_data.empty();
	_instance_struct_data.reserve(instances.size());

	for (Instance& i : instances)
		_instance_struct_data.push_back(i);
}

// =============================================================================

// Get a reference to the _polygon_struct_data vector
std::vector<Instance>* InstanceData::GetVector() {

	return &_instance_struct_data;
}

// Get the polygon data required for glBufferData
void* InstanceData::GetData() {

	return &_instance_struct_data[0];
}

// Ammount of polygon objects in the polygon buffer
int InstanceData::Size() {

	// return _polygonCount;
	return _instance_struct_data.size();
}

// Ammount of unsigned short elements in the polygon buffer (required for glDrawElements)
int InstanceData::ElementCount() {

	return _instance_struct_data.size() * 3;
}


// Data size of the Poly vector data buffer
int InstanceData::DataSize() {

	return sizeof(Instance) * _instance_struct_data.size();
}

void InstanceData::Reserve(int newPolys) {

	_instance_struct_data.empty();
	_instance_struct_data.reserve(newPolys);

	for (int x = 0; x < newPolys; x++)
		_instance_struct_data.push_back(Instance());
}