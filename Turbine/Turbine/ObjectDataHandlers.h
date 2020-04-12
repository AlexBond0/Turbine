#pragma once

#include "Matrix.h"
#include <Windows.h>
#include <vector>

// which part is being animated
enum class DataType {
	VN,		// vertex and normal data
	VNU,	// vertex, normal and uv data
	POLY	// polygon data
};

struct Polygon {

	int	point[3];
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
	float* GetData();

	int Size();
	int DataSize();

	bool HasUV();

	void SetUV(bool hasuv);


private:

	void _ConvertToPoint();
	void _ConvertToPointUV();

	bool _incuv;	// does point data include uv coords

	std::vector<Point> _point_vn_data;
	std::vector<PointUV> _point_vnu_data;

	float* _pointData;	// float* array of data ready for OpenGL
	int _pointCount;	// ammount of points in data
	int _pointDataSize;	// size of the _pointData array

	bool _pointDataSet;
};

//class PolygonData
//{
//public:
//	PolygonData();
//	~PolygonData();
//
//private:
//
//	unsigned short* polygonData;
//	int polygonCount;
//	int elementcount;
//};

