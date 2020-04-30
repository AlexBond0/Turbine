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

	void GenerateBox(float size, bool invertNormals);
	void GenerateTree(float radius, float height);
	void GenerateIcoSphere(float radius, int recursions);
	void GenerateDirector(float radius);

	static Geomitory GenerateCylinder(float radius, float height, int segments);
	static Geomitory GenerateCone(float radius, float height, int segments);
	static Geomitory GenerateCube(float size, bool invertNormals);
	static Geomitory GenerateBaseIcoSphere(float radius);

	static Geomitory GenerateCircle(float radius, int segments, bool up);

	static void TranslateGeomitory(Geomitory& geomitory, float x, float y, float z);
	static void TranslateUV(Geomitory& geomitory, float u, float v);
	static void ScaleUV(Geomitory& geomitory, float u, float v);

	void AssignGeomitoryData(const Geomitory& geomitory);


};