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
Model3D* Model3D::LoadModel(std::string filename) {

	Model3D* model = new Model3D();

	size_t lastindex = filename.find_last_of(".");
	std::string filetype = filename.substr(lastindex, filename.size());

	// is a .3dm file
	if (filetype.compare(".3dm") == 0) {

		std::wstring wideFilename = std::wstring(filename.begin(), filename.end());

		FILE* file = NULL;
		_wfopen_s(&file, wideFilename.c_str(), L"rb");

		if (file)
		{
			byte buffer[4];
			fread(buffer, 1, 4, file);
			int version = (buffer[0] & 0xFF) | ((buffer[1] & 0xFF) << 8) | ((buffer[2] & 0xFF) << 16) | ((buffer[3] & 0xFF) << 24);
			if (version == 4)
				_Read3DSVersion4(file, model);
			fclose(file);
		}
	}

	// is a .obj file
	else if (filetype.compare(".obj") == 0) {

		_ReadOBJ(filename, model);
	}

	return model;
}

// Read the data from a 3dsMax model
void Model3D::_Read3DSVersion4(FILE* file, Model3D* model)
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

void Model3D::_ReadOBJ(std::string filename, Model3D* model) {

	std::ifstream file(filename.c_str(), std::ifstream::in);

	std::string line;
	std::vector<std::string> tokens;
	std::vector<std::string> subTokens;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::string currentObj = "";

	PointData points;
	points.SetUV(true);

	PolygonData polygons;

	std::map<int, int> globalToLocalVerts;
	std::map<int, int> globalToLocalNormals;
	std::map<int, int> globalToLocalUVs;

	if (file.is_open())
	{
		while (std::getline(file, line)) {

			// get tokens
			tokens = _GetTokens(line, ' ');


			// vertices
			if (tokens[0].compare("v") == 0) {

				vertices.push_back(glm::vec3(
					std::stof(tokens[1]),
					std::stof(tokens[2]),
					std::stof(tokens[3])
				));
			}

			// normals
			else if (tokens[0].compare("vn") == 0) {

				normals.push_back(glm::vec3(
					std::stof(tokens[1]),
					std::stof(tokens[2]),
					std::stof(tokens[3])
				));
			}

			// UV coords
			else if (tokens[0].compare("vt") == 0) {

				uvs.push_back(glm::vec2(
					std::stof(tokens[1]),
					std::stof(tokens[2])
				));
			}

			// polygons
			else if (tokens[0].compare("f") == 0) {

				int numOfPolygons = tokens.size() - 3;

				// save each polygon point
				for (int tokenID = 1; tokenID < tokens.size(); tokenID++) {

					subTokens = _GetTokens(tokens[tokenID], '/');

					// save global id's to local id's
					if (globalToLocalVerts.find(std::stoi(subTokens[0])) == globalToLocalVerts.end())
						globalToLocalVerts[std::stoi(subTokens[0])] 
							= globalToLocalVerts.size();
				}

				// construct polygons
				for (int polygonID = 0; polygonID < numOfPolygons; polygonID++) {

					Poly p;

					// get global point ids
					p.point[0] = stoi(_GetTokens(tokens[1], '/')[0]);
					p.point[1] = stoi(_GetTokens(tokens[polygonID + 3], '/')[0]);
					p.point[2] = stoi(_GetTokens(tokens[polygonID + 2], '/')[0]);

					// convert global to local verts
					p.point[0] = globalToLocalVerts[p.point[0]];
					p.point[1] = globalToLocalVerts[p.point[1]];
					p.point[2] = globalToLocalVerts[p.point[2]];

					polygons.AddPolygon(p);
				}
			}

			// object name
			if (tokens[0].compare("o") == 0) {

				// save current object
				if (currentObj.compare("") != 0) {

					// go through globalToLocalVerts and construct points
					for (auto const& vertex : globalToLocalVerts) {

						PointUV p;
						p.vertex = vertices[vertex.second];

						points.AddPoint(p);
					}

					// create object
					Object3D* newobj = new Object3D(currentObj);

					// assign point and polygon data
					newobj->SetVertexData(points);
					newobj->SetTriangles(polygons);

					// save object
					model->loadedObjs.push_back(newobj);

					//clear old datastructures
					points = PointData();
					polygons = PolygonData();
					globalToLocalVerts.empty();
				}

				currentObj = tokens[1];
			}

		}



		file.close();
	}
}

std::vector<std::string> Model3D::_GetTokens(std::string line, char seperator) {

	std::vector<std::string> vec;
	std::string token = "";

	for (char t : line) {

		if (t == seperator) {

			vec.push_back(token);
			token = "";
		}
		else {

			token += t;
		}
	}

	if (token.compare("") != 0)
		vec.push_back(token);

	return vec;
}