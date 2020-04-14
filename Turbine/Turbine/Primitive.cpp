#include "Primitive.h"


Primitive::Primitive()
{
}


Primitive::~Primitive()
{
}

// generate a cube primitive
void Primitive::GenerateCube(float size, bool invertNormals) {

	// incuv = true;
	vertices.SetUV(true);

	_AssignGeomitoryData(_GenerateCube(size, invertNormals));
}

// generate a tree primitive
void Primitive::GenerateTree(float radius, float height) {

	float trunkWidth = radius / 6;
	float deciHeight = height / 10;

	// incuv = true;
	vertices.SetUV(true);

	Geomitory branchA = _GenerateCone(radius, (deciHeight * 3), 8);
	_TranslateGeomitory(branchA, 0.0f, (deciHeight * 7), 0.0f);
	_ScaleUV(branchA, 1.0f, 0.5f);
	_TranslateUV(branchA, 0.0f, 0.51f);

	Geomitory branchB = _GenerateCone(radius, (deciHeight * 3), 8);
	_TranslateGeomitory(branchB, 0.0f, (deciHeight * 6), 0.0f);
	_ScaleUV(branchB, 1.0f, 0.5f);
	_TranslateUV(branchB, 0.3f, 0.51f);

	Geomitory branchC = _GenerateCone(radius, (deciHeight * 3), 8);
	_TranslateGeomitory(branchC, 0.0f, (deciHeight * 5), 0.0f);
	_ScaleUV(branchC, 1.0f, 0.5f);
	_TranslateUV(branchC, 0.6f, 0.51f);

	Geomitory trunk		= _GenerateCylinder((radius / 6), (deciHeight * 5), 15);
	_ScaleUV(trunk, 1.0f, 0.5f);

	Geomitory tree = branchA + branchB + branchC + trunk;

	_AssignGeomitoryData(tree);


	ambient = color4(0.4, 0.4, 0.4, 1.0);
	diffuse = color4(1.0, 1.0, 1.0, 1.0);
	specular = color4(0.0, 0.0, 0.0, 1.0);
}

// generate a cylinder primitive
Geomitory Primitive::_GenerateCylinder(float radius, float height, int segments) {

	Geomitory top = _GenerateCircle(radius, segments, true);
	_TranslateGeomitory(top, 0.0f, height, 0.0f);
	_TranslateUV(top, 0.0f, 1.0f);

	Geomitory bot = _GenerateCircle(radius, segments, false);

	Geomitory topAndBot = top + bot;

	// stitch the top and bottom together
	int toBot = segments + 2;
	int botStride;
	for (int stride = 1; stride <= segments; stride++) {

		botStride = toBot + stride;

		// higher polygon
		topAndBot.polys.push_back(stride);
		topAndBot.polys.push_back((stride == segments ? 0 : stride) + 1);
		topAndBot.polys.push_back(stride == segments ? toBot : botStride);

		// lower polygon
		topAndBot.polys.push_back(stride == segments ? toBot : botStride);
		topAndBot.polys.push_back((stride == segments ? toBot + stride : botStride) - 1);
		topAndBot.polys.push_back(stride);
	}

	return topAndBot;
}

// generate a cone primitive
Geomitory Primitive::_GenerateCone(float radius, float height, int segments) {

	Geomitory base = _GenerateCircle(radius, segments, false);

	// create point vertex
	base.vecs.push_back(0.0f);	// vector
	base.vecs.push_back(height);
	base.vecs.push_back(0.0f);
	base.vecs.push_back(0.0f);	// normal
	base.vecs.push_back(1.0f);
	base.vecs.push_back(0.0f);
	base.vecs.push_back(0.5f);	// uv
	base.vecs.push_back(1.0f);

	// stitch the top point and bottom circle together
	int point = segments + 1;
	for (int stride = 1; stride <= segments; stride++) {

		// higher polygon
		base.polys.push_back(point);
		base.polys.push_back((stride == segments ? 0 : stride) + 1);
		base.polys.push_back(stride == segments ? segments : stride);
	}

	return base;
}

// generate a cube primitive
Geomitory Primitive::_GenerateCube(float size, bool invertNormals) {

	Geomitory data;

	// normal direction indicator
	int n = (invertNormals ? -1 : 1);

	// as skybox is 1 texture instead of 6,
	// black lines in skybox avoided with clamping and this:
	float X_0 = 0.001f;
	float X_1 = 0.251f;
	float X_2 = 0.499f; 
	float X_3 = 0.750f;
	float X_4 = 0.999f;

	float Y_0 = 0.001f;
	float Y_1 = 0.334f;
	float Y_2 = 0.665f;
	float Y_3 = 0.999f;

	// assuming that each vertex has 3 float, then 3 normal values, then 2 UV coords
	float verts[14][8] = {
		{ size, size, size, -n, -n, -n, X_2, Y_3 }, // 0a
		{ size, size, size, -n, -n, -n, X_3, Y_2 }, // 0b
		{ size, size, -size, -n, -n, n, X_2, Y_2 }, // 1
		{ size, -size, size, -n, n, -n, X_3, Y_1 }, // 2a
		{ size, -size, size, -n, n, -n, X_2, Y_0 }, // 2b
		{ size, -size, -size, -n, n, n, X_2, Y_1 }, // 3
		{ -size, size, size, n, -n, -n, X_1, Y_3 }, // 4a
		{ -size, size, size, n, -n, -n, X_0, Y_2 }, // 4b
		{ -size, size, size, n, -n, -n, X_4, Y_2 }, // 4c
		{ -size, size, -size, n, -n, n, X_1, Y_2 }, // 5
		{ -size, -size, size, n, n, -n, X_0, Y_1 }, // 6a
		{ -size, -size, size, n, n, -n, X_4, Y_1 }, // 6b
		{ -size, -size, size, n, n, -n, X_1, Y_0 }, // 6c
		{ -size, -size, -size, n, n, n, X_1, Y_1 }  // 7

	};
	for (int vert = 0; vert < 14; vert++)
		for (int part = 0; part < 8; part++)
			data.vecs.push_back(verts[vert][part]);

	unsigned short polys[12][3] = {
		{ 6, 9, 2 },
		{ 6, 2, 0 },
		{ 7, 10, 13 },
		{ 7, 13, 9 },
		{ 9, 13, 5 },
		{ 9, 5, 2 },
		{ 2, 5, 3 },
		{ 2, 3, 1 },
		{ 1, 3, 11 },
		{ 1, 11, 8 },
		{ 13, 12, 4 },
		{ 13, 4, 5 }
	};
	for (int poly = 0; poly < 12; poly++)
		for (int part = 0; part < 3; part++)
			data.polys.push_back(polys[poly][part]);

	return data;
}

// generate a circle primitive
Geomitory Primitive::_GenerateCircle(float radius, int segments, bool up) {

	Geomitory data;

	// create center vertex
	for (int x = 0; x < 8; x++)
		data.vecs.push_back(0.0f);

	// create vertexes
	float progress;
	for (int segment = 0; segment < segments; segment++) {

		progress = (float(segment) / segments) * (M_PI * 2);

		// vector
		data.vecs.push_back(cos(progress) * radius);
		data.vecs.push_back(0.0f);
		data.vecs.push_back(sin(progress) * radius);

		// normal
		data.vecs.push_back(cos(progress) * radius);
		data.vecs.push_back(0.0f);
		data.vecs.push_back(sin(progress) * radius);

		// uv
		data.vecs.push_back((float(segment) / segments));
		data.vecs.push_back(0.0f);
	}

	// link vertexes with polygons
	for (int vertID = 1; vertID <= segments; vertID++) {

		data.polys.push_back(up ? vertID : 0);
		data.polys.push_back(up ? 0 : vertID);
		data.polys.push_back((vertID == segments ? 0 : vertID) + 1);
	}

	return data;
}

// translate geometrical data in the Geomitory object
void Primitive::_TranslateGeomitory(Geomitory& geomitory, float x, float y, float z) {

	for (int stride = 0; stride < geomitory.vecs.size(); stride += 8) {

		geomitory.vecs[stride	 ] += x;
		geomitory.vecs[stride + 1] += y;
		geomitory.vecs[stride + 2] += z;
	}
}

// translate UV data in the Geomitory object
void Primitive::_TranslateUV(Geomitory& geomitory, float u, float v) {

	for (int stride = 0; stride < geomitory.vecs.size(); stride += 8) {

		geomitory.vecs[stride + 6] += u;
		geomitory.vecs[stride + 7] += v;
	}
}

// scale geometrical data in the Geomitory object
void Primitive::_ScaleUV(Geomitory& geomitory, float u, float v) {

	for (int stride = 0; stride < geomitory.vecs.size(); stride += 8) {

		geomitory.vecs[stride + 6] *= u;
		geomitory.vecs[stride + 7] *= v;
	}
}

// assign the geometrical data in the Geomitory object to the base Object3D class
void Primitive::_AssignGeomitoryData(const Geomitory& geomitory) {

	// assign new vertex data
	SetVertexData(
		(float*)(geomitory.vecs.data()), 
		(geomitory.vecs.size() / 8), // 8 floats define a point
		true
	);

	// assign new polygon data
	SetTriangles(
		(unsigned short*)(geomitory.polys.data()), 
		(geomitory.polys.size() / 3) // 3 unsigned shorts define a polygon
	);
}

//// combine two sets of geometrical data together
//Geomitory operator+ (Geomitory& lhs, Geomitory& rhs) {
//
//	// base first geomitory
//	Geomitory data = lhs;
//
//	// shift poly values and add to data
//	int maxVertIndex = (data.vecs.size() / 8);
//	for (unsigned short& poly : rhs.polys)
//		data.polys.push_back(poly + maxVertIndex);
//
//	// append vertex values to data
//	for (float& vert : rhs.vecs)
//		data.vecs.push_back(vert);
//
//	return data;
//}