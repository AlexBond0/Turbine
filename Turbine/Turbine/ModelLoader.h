#pragma once

#include "Object3D.h"
#include "Primitive.h"
#include <fstream>
#include <stdio.h>
#include <vector>

#include "Model.h" 

// class Model;


// A collection of Object3D elements to be rendered together, usually
// loaded from a precalculated object file
class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	static ModelLoader* LoadModel(std::string filename);

	int GetNoOfObjects();
	Model* GetModel();


private:

	Model* _model;

	static void _Read3DSVersion4(FILE* file, ModelLoader* model);
	static void _ReadOBJ(std::string filename, ModelLoader* model);

	static std::vector<std::string> _GetTokens(std::string line, char seperator);
};

inline int ModelLoader::GetNoOfObjects() {

	return _model->GetEntityCount();
}
