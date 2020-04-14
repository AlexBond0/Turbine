#include "PointData.h"

PointData::PointData() {

	_pointData = NULL;
	_pointDataSet = false;
	_incuv = false;
}


PointData::~PointData() {

	if (!_pointDataSet)
		free(_pointData);
}

// Set new point data from exiting PointData object
void PointData::SetData(PointData pointData) {

	SetData(
		pointData.GetPointerArrayData(),
		pointData.Size(),
		pointData.HasUV()
	);
}

// Set internal point memory from new byte* array
void PointData::SetData(byte* buffer, int pointCount, int bufferlen) {

	// does buffer have uv data
	_incuv = (bufferlen == 4 * (pointCount * 8));

	// allocate memory
	_pointData = (float*)malloc(bufferlen);
	_pointDataSet = true;

	// move data across
	memcpy(_pointData, buffer, bufferlen);
	_pointCount = pointCount;

	_pointDataSize = sizeof(float) * _pointCount * (_incuv ? 8 : 6);

	if (_incuv)
		_ConvertToPointUV();
	else
		_ConvertToPoint();
}

// Set internal point memory from new float* array
void PointData::SetData(float* newVertData, int pointCount, bool uv) {

	_incuv = uv;

	_pointDataSize = sizeof(float) * pointCount * (uv ? 8 : 6);

	// allocate memory
	free(_pointData);
	_pointData = (float*)malloc(_pointDataSize);
	_pointDataSet = true;

	// move data across
	memcpy(_pointData, newVertData, _pointDataSize);
	_pointCount = pointCount;


	if (_incuv)
		_ConvertToPointUV();
	else
		_ConvertToPoint();
}

// Convert current _pointData heap memeory into useable std::vector<Point>
void PointData::_ConvertToPoint() {

	int stride;
	glm::vec3	vertex;
	glm::vec3	normal;
	_point_vn_data.empty();
	_point_vn_data.reserve(_pointCount);

	for (int point = 0; point < _pointCount; point++) {

		// 6 floats define a Point
		stride = (point * 6);

		vertex = glm::vec3(
			_pointData[stride + 0],
			_pointData[stride + 1],
			_pointData[stride + 2]
		);

		normal = glm::vec3(
			_pointData[stride + 3],
			_pointData[stride + 4],
			_pointData[stride + 5]
		);

		Point p;
		p.vertex = vertex;
		p.normal = normal;

		_point_vn_data.push_back(p);
	}
}

// Convert current _pointData heap memeory into useable std::vector<PointUV>
void PointData::_ConvertToPointUV() {

	int stride;
	glm::vec3	vertex;
	glm::vec3	normal;
	glm::vec2	uv;
	_point_vnu_data.empty();
	_point_vnu_data.reserve(_pointCount);

	for (int point = 0; point < _pointCount; point++) {


		stride = (point * 8);

		vertex = glm::vec3(
			_pointData[stride + 0],
			_pointData[stride + 1],
			_pointData[stride + 2]
		);

		normal = glm::vec3(
			_pointData[stride + 3],
			_pointData[stride + 4],
			_pointData[stride + 5]
		);

		uv = glm::vec2(
			_pointData[stride + 6],
			_pointData[stride + 7]
		);

		PointUV p;
		p.vertex = vertex;
		p.normal = normal;
		p.uv = uv;

		_point_vnu_data.push_back(p);
	}
}

// Add new point, returns false if Point was modified to PointUV to be added correctly
bool PointData::AddPoint(Point newPoint) {

	// incoming newPoint needs no modification
	if (!_incuv) {

		_point_vn_data.push_back(newPoint);
		return true;
	}

	// newPoint needs converting
	else {

		PointUV p;
		p.vertex = newPoint.vertex;
		p.normal = newPoint.normal;
		p.uv = glm::vec2(1.0);

		_point_vnu_data.push_back(p);
		return false;
	}
}

// Add new point, returns false if PointUV was modified to Point to be added correctly
bool PointData::AddPoint(PointUV newPoint) {

	// incoming newPoint needs no modification
	if (_incuv) {

		_point_vnu_data.push_back(newPoint);
		return true;
	}

	// newPoint needs converting
	else {

		Point p;
		p.vertex = newPoint.vertex;
		p.normal = newPoint.normal;

		_point_vn_data.push_back(p);
		return false;
	}
}

// Gets PointUV at given index (may return nullptr if wrong datatype is requested)
PointUV* PointData::GetPointUV(int index) {

	// if index is in range and has uv coords
	if (_incuv && index >= 0 && index < _point_vnu_data.size())
		return &_point_vnu_data[index];

	else
		return nullptr;
}

// Gets Point at given index (may return nullptr if wrong datatype is requested)
Point* PointData::GetPoint(int index) {

	// if index is in range and has no uv coords
	if (!_incuv && index >= 0 && index < _point_vn_data.size())
		return &_point_vn_data[index];

	else
		return nullptr;
}

// Delete point at index, returns true if completed correctly
bool PointData::DeletePoint(int index) {

	if (_incuv) {

		if (index >= 0 && index < _point_vnu_data.size()) {

			_point_vnu_data.erase(_point_vnu_data.begin() + index);
			return true;
		}
		return false;
	}
	else {

		if (index >= 0 && index < _point_vn_data.size()) {

			_point_vn_data.erase(_point_vn_data.begin() + index);
			return true;
		}
		return false;
	}
}


// ==============================================================================

// Get the polygon data required for glBufferData
// NEEDS DEPRECIATING
float* PointData::GetPointerArrayData() {

	return _pointData;
}

// Get the polygon data required for glBufferData
void* PointData::GetVectorData() {

	if (_incuv)
		return &_point_vnu_data[0];

	else
		return &_point_vn_data[0];
}

// Ammount of point objects in the point buffer
int PointData::Size() {

	return _pointCount;
}

// Data length of the _pointData heap memeory
// NEEDS DEPRECIATING
int PointData::VectorDataSize() {

	return _pointDataSize;
}

// Data length of the point vector
int PointData::PointerArrayDataSize() {

	if (_incuv)
		return sizeof(PointUV) * _point_vnu_data.size();

	else
		return sizeof(Point) * _point_vn_data.size();
}

// Does the point data have uv coordinates
bool PointData::HasUV() {

	return _incuv;
}

// Define if point data has UV coordinates
// NOT RECCOMENDED TO SET UNLESS NEW DATA IS BEING ASSIGNED
void PointData::SetUV(bool hasuv) {

	_incuv = hasuv;
}
