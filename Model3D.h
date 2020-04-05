#pragma once
#include "Object3D.h"
#include "Primitive.h"
#include <stdio.h>

class Model3D
{
public:
	Model3D();
	~Model3D();

	static Model3D* LoadModel(const wchar_t* filename);

	void Draw(RenderingContext rcontext);

	int GetNoOfObjects();
	Object3D** GetObjects();

	Object3D* rootObject = nullptr;

private:
	int noofobjects;
	Object3D** objects;

	static void Model3D::Read3DSVersion4(FILE* file, Model3D* model);
};

inline int Model3D::GetNoOfObjects()
{
	return noofobjects;
}

inline Object3D** Model3D::GetObjects()
{
	return objects;
}

inline void Model3D::Draw(RenderingContext rcontext)
{
	if (rootObject == nullptr) {

		for (int i = 0; i < noofobjects; i++)
			objects[i]->Draw(rcontext);
	}
	else {

		rootObject->Draw(rcontext);
	}
}