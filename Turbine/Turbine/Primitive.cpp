#include "Primitive.h"


Primitive::Primitive()
	: Object3D("PRIMITIVE")
{
	SetEntityType(EntityType::OBJ_PRIMITIVE);
	vertices.SetUV(true);
}


Primitive::~Primitive()
{
}

// generate a cube primitive
void Primitive::GenerateBox(float size, bool invertNormals) {

	SetName("CUBE_PRIMITIVE");

	// incuv = true;
	vertices.SetUV(true);

	AssignGeomitoryData(GenerateCube(size, invertNormals));
}

// generate a tree primitive
void Primitive::GenerateTree(float radius, float height) {

	SetName("TREE_PRIMITIVE");

	float trunkWidth = radius / 6;
	float deciHeight = height / 10;

	// incuv = true;
	vertices.SetUV(true);

	Geomitory branchA = GenerateCone(radius, (deciHeight * 3), 8);
	TranslateGeomitory(branchA, 0.0f, (deciHeight * 7), 0.0f);
	ScaleUV(branchA, 1.0f, 0.5f);
	TranslateUV(branchA, 0.0f, 0.51f);

	Geomitory branchB = GenerateCone(radius, (deciHeight * 3), 8);
	TranslateGeomitory(branchB, 0.0f, (deciHeight * 6), 0.0f);
	ScaleUV(branchB, 1.0f, 0.5f);
	TranslateUV(branchB, 0.3f, 0.51f);

	Geomitory branchC = GenerateCone(radius, (deciHeight * 3), 8);
	TranslateGeomitory(branchC, 0.0f, (deciHeight * 5), 0.0f);
	ScaleUV(branchC, 1.0f, 0.5f);
	TranslateUV(branchC, 0.6f, 0.51f);

	Geomitory trunk		= GenerateCylinder((radius / 6), (deciHeight * 5), 15);
	ScaleUV(trunk, 1.0f, 0.5f);

	Geomitory tree = branchA + branchB + branchC + trunk;

	AssignGeomitoryData(tree);


	SetAmbient(color4(0.4, 0.4, 0.4, 1.0));
	SetDiffuse(color4(1.0, 1.0, 1.0, 1.0));
	SetSpecular(color4(0.0, 0.0, 0.0, 1.0));
}

void Primitive::GenerateIcoSphere(float radius, int recursions) {

	SetName("ICOSPHERE_PRIMITIVE");

	vertices.SetUV(true);

	Geomitory sphere = GenerateBaseIcoSphere(radius);
	AssignGeomitoryData(sphere);

	SetAmbient(color4(0.4, 0.4, 0.4, 1.0));
	SetDiffuse(color4(1.0, 1.0, 1.0, 1.0));
	SetSpecular(color4(0.0, 0.0, 0.0, 1.0));
}

void Primitive::GenerateDirector(float radius) {

	SetName("DIRECTOR_PRIMITIVE");

	vertices.SetUV(true);

	Geomitory sphere = GenerateBaseIcoSphere(radius);

	Geomitory pointer = GenerateCylinder((radius / 3), (radius * 2), 4);
	TranslateGeomitory(pointer, 0.0f, radius, 0.0f);

	Geomitory director = sphere + pointer;

	AssignGeomitoryData(director);

	SetAmbient(color4(0.4, 0.4, 0.4, 1.0));
	SetDiffuse(color4(1.0, 1.0, 1.0, 1.0));
	SetSpecular(color4(0.0, 0.0, 0.0, 1.0));
}


// generate a cylinder primitive
Geomitory Primitive::GenerateCylinder(float radius, float height, int segments) {

	Geomitory top = GenerateCircle(radius, segments, true);
	TranslateGeomitory(top, 0.0f, height, 0.0f);
	TranslateUV(top, 0.0f, 1.0f);

	Geomitory bot = GenerateCircle(radius, segments, false);

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
Geomitory Primitive::GenerateCone(float radius, float height, int segments) {

	Geomitory base = GenerateCircle(radius, segments, false);

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
Geomitory Primitive::GenerateCube(float size, bool invertNormals) {

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

Geomitory Primitive::GenerateBaseIcoSphere(float radius) {

	Geomitory data;

	float t = (1.0 + sqrt(5.0)) / 2.0;
	t *= radius;

	float verts[12][8] = {
		{ -radius, t, 0, -radius, t, 0, 0, 0 },
		{ radius, t, 0, radius, t, 0, 0, 0 },
		{ -radius, -t, -0, -radius, -t, -0, 0, 0 },
		{ radius, -t, 0, radius, -t, 0, 0, 0 },
		{ 0, -radius, t, 0, -radius, t, 0, 0 },
		{ 0, radius, t, 0, radius, t, 0, 0 },
		{ 0, -radius, -t, 0, -radius, -t, 0, 0 },
		{ 0, radius, -t, 0, radius, -t, 0, 0 },
		{ t, 0, -radius, t, 0, -radius, 0, 0 },
		{ t, 0, radius, t, 0, radius, 0, 0 },
		{ -t, 0, -radius, -t, 0, -radius, 0, 0 },
		{ -t, 0, radius, -t, 0, radius, 0, 0 },

	};
	for (int vert = 0; vert < 12; vert++)
		for (int part = 0; part < 8; part++)
			data.vecs.push_back(verts[vert][part]);

	unsigned short polys[20][3] = {
		{11, 5, 0},
		{5, 1, 0},
		{1, 7, 0},
		{7, 10, 0},
		{10, 11, 0},

		// 5 adjacent faces
		{5, 9, 1},
		{11, 4, 5},
		{10, 2, 11},
		{7, 6, 10},
		{1, 8, 7},

		// 5 faces around point 3
		{9, 4, 3},
		{4, 2, 3},
		{2, 6, 3},
		{6, 8, 3},
		{8, 9, 3},

		// 5 adjacent faces
		{9, 5, 4},
		{4, 11, 2},
		{2, 10, 6},
		{6, 7, 8},
		{8, 1, 9}
	};
	for (int poly = 0; poly < 20; poly++)
		for (int part = 0; part < 3; part++)
			data.polys.push_back(polys[poly][part]);

	return data;
}

// generate a circle primitive
Geomitory Primitive::GenerateCircle(float radius, int segments, bool up) {

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

Geomitory Primitive::GeneratePlane(float width, int height, bool doubleSided = false) {

	Geomitory data;

	// vertices
	float verts[4][8] = {
		{ (-width * 0.5), 0, (-height * 0.5), (-width * 0.5), 0, (-height * 0.5), 0, 0 },
		{ (width * 0.5), 0, (-height * 0.5), (width * 0.5), 0, (-height * 0.5), 1, 0 },
		{ (-width * 0.5), 0, (height * 0.5), (-width * 0.5), 0, (height * 0.5), 0, 1 },
		{ (width * 0.5), 0, (height * 0.5), (width * 0.5), 0, (height * 0.5), 1, 1 }
	};
	for (int vert = 0; vert < 4; vert++)
		for (int part = 0; part < 8; part++)
			data.vecs.push_back(verts[vert][part]);

	// uvs
	if (doubleSided) {

		unsigned short polys[4][3] = {
			{0, 1, 2},
			{1, 3, 2},
			{2, 1, 0},
			{2, 3, 1}
		};
		for (int poly = 0; poly < 4; poly++)
			for (int part = 0; part < 3; part++)
				data.polys.push_back(polys[poly][part]);
	}
	else {

		unsigned short polys[2][3] = {
			{0, 1, 2},
			{1, 3, 2}
		};
		for (int poly = 0; poly < 2; poly++)
			for (int part = 0; part < 3; part++)
				data.polys.push_back(polys[poly][part]);
	}


	return data;
}

// translate geometrical data in the Geomitory object
void Primitive::TranslateGeomitory(Geomitory& geomitory, float x, float y, float z) {

	for (int stride = 0; stride < geomitory.vecs.size(); stride += 8) {

		geomitory.vecs[stride	 ] += x;
		geomitory.vecs[stride + 1] += y;
		geomitory.vecs[stride + 2] += z;
	}
}

// translate UV data in the Geomitory object
void Primitive::TranslateUV(Geomitory& geomitory, float u, float v) {

	for (int stride = 0; stride < geomitory.vecs.size(); stride += 8) {

		geomitory.vecs[stride + 6] += u;
		geomitory.vecs[stride + 7] += v;
	}
}

// scale geometrical data in the Geomitory object
void Primitive::ScaleUV(Geomitory& geomitory, float u, float v) {

	for (int stride = 0; stride < geomitory.vecs.size(); stride += 8) {

		geomitory.vecs[stride + 6] *= u;
		geomitory.vecs[stride + 7] *= v;
	}
}

// assign the geometrical data in the Geomitory object to the base Object3D class
void Primitive::AssignGeomitoryData(const Geomitory& geomitory) {

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