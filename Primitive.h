#pragma once

#include "Object3D.h"
#include <vector>

struct Geomitory {

	std::vector<float> vecs;
	std::vector<unsigned short> polys;
};

Geomitory operator+(Geomitory& lhs, Geomitory& rhs);

class Primitive :
	public Object3D
{
public:
	Primitive();
	~Primitive();

	void GenerateCube(float size, bool invertNormals);
	void GenerateTree(float radius, float height);

private:

	Geomitory _GenerateCylinder(float radius, float height, int segments);
	Geomitory _GenerateCone(float radius, float height, int segments);
	Geomitory _GenerateCube(float size, bool invertNormals);

	Geomitory _GenerateCircle(float radius, int segments, bool up);

	void _TranslateGeomitory(Geomitory& geomitory, float x, float y, float z);
	void _TranslateUV(Geomitory& geomitory, float u, float v);
	void _ScaleUV(Geomitory& geomitory, float u, float v);

	void _AssignGeomitoryData(const Geomitory& geomitory);
};