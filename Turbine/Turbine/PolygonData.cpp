#include "PolygonData.h"


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

// Assign polygon data from an existing PolygonData object
void PolygonData::SetData(PolygonData polygonData) {

	SetData(
		polygonData.GetPointerArrayData(),
		polygonData.Size()
	);
}

// Assign polygon data to the Object, from file load
void PolygonData::SetData(byte* fileBuffer, int noofElements) {

	_SetData((unsigned short*)fileBuffer, noofElements);
}

// Assign polygon data to the Object
void PolygonData::SetData(unsigned short* newPolyData, int noofElements) {

	_SetData(newPolyData, noofElements);
}

// convert the stored pointer array data to the struct storage
void PolygonData::_ConvertToPolygon() {

	int stride;
	_polygon_struct_data.empty();

	for (int point = 0; point < _polygonCount; point++) {

		stride = point * 3;

		Poly p;
		p.point[0] = _polygonData[stride + 0];
		p.point[1] = _polygonData[stride + 1];
		p.point[2] = _polygonData[stride + 2];

		_polygon_struct_data.push_back(p);
	}
}

// Assign polygon data to the Object
void PolygonData::_SetData(unsigned short* newPolyData, int noofElements) {

	_polygonDataSize = 3 * noofElements * sizeof(unsigned short);

	free(_polygonData);
	_polygonData = (unsigned short*)malloc(_polygonDataSize);
	memcpy(_polygonData, newPolyData, _polygonDataSize);
	_polygonDataSet = true;

	_polygonElementType = GL_TRIANGLES;
	_polygonCount = noofElements;

	_ConvertToPolygon();
}

// =============================================================================

// Get the polygon data required for glBufferData
// NEEDS DEPRECIATING
unsigned short* PolygonData::GetPointerArrayData() {

	return _polygonData;
}

// Get the polygon data required for glBufferData
unsigned short* PolygonData::GetVectorData() {

	return (unsigned short*)&_polygon_struct_data[0];
}

// Ammount of polygon objects in the polygon buffer
int PolygonData::Size() {

	return _polygonCount;
}

// Ammount of unsigned short elements in the polygon buffer (required for glDrawElements)
int PolygonData::ElementCount() {

	return _polygonCount * 3;
}

// Data size of the pointer array data buffer
int PolygonData::PointerArrayDataSize() {

	return _polygonDataSize;
}

// Data size of the Poly vector data buffer
int PolygonData::VectorDataSize() {

	return sizeof(Poly) * _polygon_struct_data.size();
}

// Render element type for OpenGL
int PolygonData::ElementType() {

	return _polygonElementType;
}