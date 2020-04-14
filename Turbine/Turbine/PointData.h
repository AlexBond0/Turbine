#pragma once
#include "Matrix.h"
#include <Windows.h>
#include <vector>
#include "GLSetup.h"

// struct defining layout of a point with vertex, normal and uv coordinates
struct PointUV {

	// KEEP LAYOUT FOR DATA ALLINGMENT
	// ==================
	glm::vec3	vertex;
	glm::vec3	normal;
	glm::vec2	uv;
	// ==================
};

// struct defining layout of a point with vertex and normal coordinates
struct Point {

	// KEEP LAYOUT FOR DATA ALLINGMENT
	// ==================
	glm::vec3	vertex;
	glm::vec3	normal;
	// ==================
};

// Data structure to manage the internal Point data for an object
class PointData {
public:
	PointData();
	~PointData();

	void SetData(PointData pointData);
	void SetData(byte* buffer, int noofverts, int bufferlen);
	void SetData(float* newVertData, int noofverts, bool uv);
	void SetUV(bool hasuv);

	// float* GetPointerArrayData();
	void* GetData();

	int Size();

	bool AddPoint(Point newPoint);
	bool AddPoint(PointUV newPoint);

	PointUV* GetPointUV(int index);
	Point* GetPoint(int index);

	bool DeletePoint(int index);

	// int VectorDataSize();
	int DataSize();
	bool HasUV();

private:

	void _ConvertToPoint(float* pointData, int pointCount);
	void _ConvertToPointUV(float* pointData, int pointCount);

	bool _incuv;	// does point data include uv coords

	// only one of these will be active at any time, and the PointData class
	// manages data in and out of these vectors, keeping them private

	std::vector<Point> _point_vn_data;		// Storage of Point elements defining the object
	std::vector<PointUV> _point_vnu_data;	// Storage of PointUV elements defining the object

	// float* _pointData;	// float* array of data ready for OpenGL
	// int _pointCount;	// ammount of points in data
	// int _pointDataSize;	// size of the _pointData array

	// bool _pointDataSet;	// Has the _pointData pointer been set | NEEDS DEPRECIATING
};