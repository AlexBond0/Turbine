#pragma once
#include "Object3D.h"
#include "Primitive.h"
#include <fstream>
#include <stdio.h>
#include <vector>

// A collection of Object3D elements to be rendered together, usually
// loaded from a precalculated object file
class Model3D
{
public:
	Model3D();
	~Model3D();

	static Model3D* LoadModel(std::string filename);

	void Draw(RenderingContext& rcontext);

	int GetNoOfObjects();
	// Object3D** GetObjects();
	std::vector<Object3D*> GetObjects();

	Object3D* rootObject = nullptr;

private:
	// int noofobjects;
	// Object3D** objects;

	std::vector<Object3D*> _objects;

	static void _Read3DSVersion4(FILE* file, Model3D* model);
	static void _ReadOBJ(std::string filename, Model3D* model);

	static std::vector<std::string> _GetTokens(std::string line, char seperator);
};

inline int Model3D::GetNoOfObjects()
{
	return _objects.size();
}

inline std::vector<Object3D*> Model3D::GetObjects()
{
	return _objects;
}

inline void Model3D::Draw(RenderingContext& rcontext)
{
	if (rootObject == nullptr) {

		for (int i = 0; i < _objects.size(); i++)
			_objects[i]->Draw(rcontext);
	}
	else {

		rootObject->Draw(rcontext);
	}
}