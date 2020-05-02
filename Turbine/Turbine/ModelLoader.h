#pragma once
#include "Object3D.h"
#include "Primitive.h"
#include <fstream>
#include <stdio.h>
#include <vector>

// A collection of Object3D elements to be rendered together, usually
// loaded from a precalculated object file
class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	static ModelLoader* LoadModel(std::string filename);

	void Draw(RenderingContext& rcontext);

	int GetNoOfObjects();
	std::vector<Object3D*> GetObjects();

	Object3D* rootObject = nullptr;

private:

	std::vector<Object3D*> _objects;

	static void _Read3DSVersion4(FILE* file, ModelLoader* model);
	static void _ReadOBJ(std::string filename, ModelLoader* model);

	static std::vector<std::string> _GetTokens(std::string line, char seperator);
};

inline int ModelLoader::GetNoOfObjects()
{
	return _objects.size();
}

inline std::vector<Object3D*> ModelLoader::GetObjects()
{
	return _objects;
}

inline void ModelLoader::Draw(RenderingContext& rcontext)
{
	if (rootObject == nullptr) {

		for (int i = 0; i < _objects.size(); i++)
			_objects[i]->Draw(rcontext);
	}
	else {

		rootObject->Draw(rcontext);
	}
}