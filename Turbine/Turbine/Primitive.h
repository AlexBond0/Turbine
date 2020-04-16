#pragma once

#include "Object3D.h"
#include <vector>

struct Geomitory {

	std::vector<float> vecs;
	std::vector<unsigned short> polys;

	// combine two sets of geometrical data together
	Geomitory operator+ (Geomitory rhs) {

		// base first geomitory
		Geomitory data = *this;

		// shift poly values and add to data
		int maxVertIndex = (data.vecs.size() / 8);
		for (unsigned short& poly : rhs.polys)
			data.polys.push_back(poly + maxVertIndex);

		// append vertex values to data
		for (float& vert : rhs.vecs)
			data.vecs.push_back(vert);

		return data;
	}
};



// An Object3D with pre-defined geometory & geometory creation tools
class Primitive :
	public Object3D
{
public:
	Primitive();
	~Primitive();

	void GenerateCube(float size, bool invertNormals);
	void GenerateTree(float radius, float height);
	void GenerateIcoSphere(float radius, int recursions);
	void GenerateDirector(float radius);

private:

	Geomitory _GenerateCylinder(float radius, float height, int segments);
	Geomitory _GenerateCone(float radius, float height, int segments);
	Geomitory _GenerateCube(float size, bool invertNormals);
	Geomitory _GenerateBaseIcoSphere(float radius);

	Geomitory _GenerateCircle(float radius, int segments, bool camUp);

	void _TranslateGeomitory(Geomitory& geomitory, float x, float y, float z);
	void _TranslateUV(Geomitory& geomitory, float u, float v);
	void _ScaleUV(Geomitory& geomitory, float u, float v);

	void _AssignGeomitoryData(const Geomitory& geomitory);
};