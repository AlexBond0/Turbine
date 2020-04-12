#include "ObjectDataHandlers.h"

PointData::PointData() {

	_pointData = NULL;
	_pointDataSet = false;
	_incuv = false;
}


PointData::~PointData() {
	
	if (!_pointDataSet)
		free(_pointData);
}

void PointData::SetData(PointData pointData) {

	SetData(
		pointData.GetData(),
		pointData.Size(),
		pointData.HasUV()
	);
}

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

void PointData::_ConvertToPoint() {

	int stride;
	glm::vec3	vertex;
	glm::vec3	normal;
	_point_vn_data.empty();

	for (int point = 0; point < _pointCount; point++) {

		stride = point * 6;

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

void PointData::_ConvertToPointUV() {

	int stride;
	glm::vec3	vertex;
	glm::vec3	normal;
	glm::vec2	uv;
	_point_vnu_data.empty();

	for (int point = 0; point < _pointCount; point++) {

		stride = point * 6;

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


// ==============================================================================

float* PointData::GetData() {

	return _pointData;
}

int PointData::Size() {
	
	return _pointCount;
}

int PointData::DataSize() {

	return _pointDataSize;
}

bool PointData::HasUV() {

	return _incuv;
}

void PointData::SetUV(bool hasuv) {

	_incuv = hasuv;
}

// =============================================================================

PolygonData::PolygonData() {

	_polygonData = NULL;
	_polygonCount = 0;
	_polygonDataSet = false;
	_polygonElementType = GL_TRIANGLES;
}

PolygonData::~PolygonData() {

	if (!_polygonDataSet)
		free(_polygonData);
}


void PolygonData::SetData(PolygonData polygonData) {

	SetData(
		polygonData.GetData(),
		polygonData.Size()
	);
}

void PolygonData::SetData(byte* buffer, int noofpolys) {

	_polygonDataSize = 3 * noofpolys * sizeof(unsigned short);

	_polygonData = (unsigned short*)malloc(_polygonDataSize);
	memcpy(_polygonData, buffer, _polygonDataSize);
	_polygonDataSet = true;

	_polygonElementType = GL_TRIANGLES;
	_polygonCount = 3 * noofpolys;
}

// Assign polygon data to the Object
void PolygonData::SetData(unsigned short* newPolyData, int noofElements) {

	_polygonDataSize = noofElements * sizeof(unsigned short);

	free(_polygonData);
	_polygonData = (unsigned short*)malloc(_polygonDataSize);
	memcpy(_polygonData, newPolyData, _polygonDataSize);
	_polygonDataSet = true;

	_polygonElementType = GL_TRIANGLES;
	_polygonCount = noofElements;
}


// =============================================================================

unsigned short* PolygonData::GetData() {

	return _polygonData;
}

int PolygonData::Size() {

	return _polygonCount;
}

int PolygonData::DataSize() {

	return _polygonDataSize;
}

int PolygonData::ElementType() {

	return _polygonElementType;
}