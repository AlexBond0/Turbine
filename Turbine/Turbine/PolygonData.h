#pragma once

#include "Matrix.h"
#include <Windows.h>
#include <vector>
#include "GLSetup.h"

// struct defining layout of a polygon
struct Poly {

	unsigned short point[3];
};

// Data structure to manage the internal polygon data for an object
class PolygonData
{
public:
	PolygonData();
	~PolygonData();

	void SetData(PolygonData pointData);
	void SetData(byte* fileBuffer, int noofElements);
	void SetData(unsigned short* polygons, int noofElements);

	std::vector<Poly>* GetVector();
	unsigned short* GetData();

	int Size();
	int ElementCount();
	int DataSize();
	int ElementType();

private:

	int _polygonElementType;				// Type of OpenGL polygon element
	std::vector<Poly> _polygon_struct_data;	// Storage of Poly elements defining the object

	void _SetData(unsigned short* polygons, int noofElements);
	void _ConvertToPolygon(unsigned short* polygonData, int polygonCount);
};

