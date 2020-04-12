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
	int length = sizeof(float) * pointCount * (uv ? 8 : 6);

	// allocate memory
	free(_pointData);
	_pointData = (float*)malloc(length);
	_pointDataSet = true;

	// move data across
	memcpy(_pointData, newVertData, length);
	_pointCount = pointCount;

	_pointDataSize = sizeof(float) * _pointCount * (_incuv ? 8 : 6);

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

void PointData::SetData(PointData pointData) {

	SetData(
		pointData.GetData(),
		pointData.Size(),
		pointData.HasUV()
	);
}

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