#pragma once

#include "Matrix.h"
#include <Windows.h>
#include <vector>
#include "GLSetup.h"

// struct defining layout of a polygon
struct Poly {

	unsigned short point[3];
};

class PolygonData
{
public:
	PolygonData();
	~PolygonData();

	void SetData(PolygonData pointData);
	void SetData(byte* fileBuffer, int noofElements);
	void SetData(unsigned short* polygons, int noofElements);

	// unsigned short* GetPointerArrayData();
	unsigned short* GetData();


	int Size();
	int ElementCount();
	int DataSize();
	// int PointerArrayDataSize();
	int ElementType();

private:

	// unsigned short* _polygonData;	// Raw heap memory representation of polygons | NEEDS DEPRECIATING
	// int _polygonCount;				// Ammount of polygons in raw heap memory | NEEDS DEPRECIATING
	// int _polygonDataSize;			// size of data stroed in heap memory | MIGHT NEED DEPRECATING
	int _polygonElementType;		// Type of OpenGL polygon element

	bool _polygonDataSet;			// Has the _polygonData pointer been set | NEEDS DEPRECIATING

	std::vector<Poly> _polygon_struct_data;	// Storage of Poly elements defining the object

	void _SetData(unsigned short* polygons, int noofElements);
	void _ConvertToPolygon(unsigned short* polygonData, int polygonCount);
};

