#pragma once

#include "Matrix.h"
#include <Windows.h>
#include <vector>
#include "GLSetup.h"

// which part is being animated
//enum class DataType {
//	VN,		// vertex and normal data
//	VNU,	// vertex, normal and uv data
//	POLY	// polygon data
//};

#define OFFSET_VERTEX 0
#define OFFSET_NORMAL sizeof(glm::vec3)
#define OFFSET_UV (OFFSET_NORMAL + sizeof(glm::vec3))

struct Poly {

	unsigned short point[3];
};

struct PointUV {

	// KEEP LAYOUT FOR DATA ALLINGMENT
	// ==================
	glm::vec3	vertex;
	glm::vec3	normal;
	glm::vec2	uv;
	// ==================
};

struct Point {

	// KEEP LAYOUT FOR DATA ALLINGMENT
	// ==================
	glm::vec3	vertex;
	glm::vec3	normal;
	// ==================
};

class PointData {
public:
	PointData();
	~PointData();

	void SetData(PointData pointData);
	void SetData(byte* buffer, int noofverts, int bufferlen);
	void SetData(float* newVertData, int noofverts, bool uv);
	void SetUV(bool hasuv);

	float* GetPointerArrayData();
	void* GetVectorData();

	int Size();

	int VectorDataSize();
	int PointerArrayDataSize();
	bool HasUV();

	std::vector<Point> _point_vn_data;
	std::vector<PointUV> _point_vnu_data;
private:

	void _ConvertToPoint();
	void _ConvertToPointUV();

	bool _incuv;	// does point data include uv coords

	

	float* _pointData;	// float* array of data ready for OpenGL
	int _pointCount;	// ammount of points in data
	int _pointDataSize;	// size of the _pointData array

	bool _pointDataSet;
};

class PolygonData
{
public:
	PolygonData();
	~PolygonData();

	void SetData(PolygonData pointData);
	void SetData(byte* fileBuffer, int noofElements);
	void SetData(unsigned short* polygons, int noofElements);

	unsigned short* GetPointerArrayData();
	unsigned short* GetVectorData();


	int Size();
	int ElementCount();
	int VectorDataSize();
	int PointerArrayDataSize();
	int ElementType();

private:

	unsigned short* _polygonData;
	int _polygonCount;
	int _polygonDataSize;
	int _polygonElementType;

	bool _polygonDataSet;

	std::vector<Poly> _polygon_struct_data;

	void _SetData(unsigned short* polygons, int noofElements);
	void _ConvertToPolygon();
};

