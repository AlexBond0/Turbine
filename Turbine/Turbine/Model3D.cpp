#include "Model3D.h"

Model3D::Model3D()
{
	noofobjects = 0;
	objects = NULL;
}

Model3D::~Model3D()
{
	free(objects);
}

// Load a 3dsMax model into the scene
Model3D* Model3D::LoadModel(const wchar_t* filename)
{
	Model3D* model = new Model3D();

	FILE* file = NULL;
	_wfopen_s(&file, filename, L"rb");

	if (file)
	{
		byte buffer[4];
		fread(buffer, 1, 4, file);
		int version = (buffer[0] & 0xFF) | ((buffer[1] & 0xFF) << 8) | ((buffer[2] & 0xFF) << 16) | ((buffer[3] & 0xFF) << 24);
		if (version == 4)
			Read3DSVersion4(file, model);
		fclose(file);
	}

	return model;
}

// Read the data from a 3dsMax model
void Model3D::Read3DSVersion4(FILE* file, Model3D* model)
{
	int config = 0;   // if the file includes texture coordinates then config&1=1
	int bufflen = 60;
	byte* buffer = (byte*)malloc(bufflen);  // enough for a bulk material read
	int read = fread(buffer, 1, 4, file);
	if (read == 4)
		config = *(int*)buffer;

	read = fread(buffer, 1, 4, file);
	if (read == 4)
	{
		model->noofobjects = *(int*)buffer;
		model->objects = (Object3D**)malloc(sizeof(Object3D*)*model->noofobjects);
	}

	for (int i = 0; i<model->noofobjects; i++)
	{
		Object3D* object = model->objects[i] = new Object3D("OBJ_FROM_FILE");
		fread(buffer, 1, 4, file);
		int len = *(int*)buffer;
		fread(buffer, 1, len, file);

		buffer[len] = '\0';
		object->SetName((char*)buffer);

		fread(buffer, 1, 8, file);
		int noofverts = *(int*)buffer;
		int noofpolys = *(int*)(buffer + 4);

		int reqsize = noofverts * 6;
		if ((config & 1) == 1)
			reqsize += noofverts * 2; // uv coordindates
		reqsize *= 4;

		if (bufflen<reqsize)
		{
			free(buffer);
			buffer = (byte*)malloc(reqsize);
			bufflen = reqsize;
		}

		int pos = 0;
		do
		{
			read = fread(buffer + pos, 1, reqsize - pos, file);
			if (read>0)
				pos += read;
		} while (read>0 && pos<reqsize);
		object->SetVertexData(buffer, noofverts, reqsize);

		reqsize = noofpolys * 6;
		if (bufflen<reqsize)
		{
			free(buffer);
			buffer = (byte*)malloc(reqsize);
			bufflen = reqsize;
		}

		pos = 0;
		do
		{
			read = fread(buffer + pos, 1, reqsize - pos, file);
			if (read>0)
				pos += read;
		} while (read>0 && pos<reqsize);
		object->SetTriangles(buffer, noofpolys);

		fread(buffer, 1, 12, file);
		object->SetWorldPos(buffer);

		fread(buffer, 1, 52, file);
		object->SetMaterial(buffer);
	}

	free(buffer);
}