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
}

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