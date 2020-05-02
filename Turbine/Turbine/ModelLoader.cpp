#include "ModelLoader.h"

ModelLoader::ModelLoader() {

}

ModelLoader::~ModelLoader() {

}

// Load a 3dsMax model into the scene
ModelLoader* ModelLoader::LoadModel(std::string filename) {

	ModelLoader* model = new ModelLoader();

	size_t lastindex = filename.find_last_of(".");
	std::string filetype = filename.substr(lastindex, filename.size());

	// is a .3dm file
	if (filetype.compare(".3dm") == 0) {

		// get model name
		std::vector<std::string> modelName = _GetTokens(filename, '.');
		model->_model = new Model(modelName[0]);

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
void ModelLoader::_Read3DSVersion4(FILE* file, ModelLoader* model) {

	int config = 0;   // if the file includes texture coordinates then config&1=1
	int bufflen = 60;
	byte* buffer = (byte*)malloc(bufflen);  // enough for a bulk material read
	int read = fread(buffer, 1, 4, file);
	if (read == 4)
		config = *(int*)buffer;

	int noofobjects;

	// Object3D** objs = NULL;

	read = fread(buffer, 1, 4, file);
	if (read == 4)
	{
		noofobjects = *(int*)buffer;
		// Object3D** objs = (Object3D**)malloc(sizeof(Object3D*)*model->noofobjects);
	}

	for (int i = 0; i<noofobjects; i++)
	{

		Object3D* object = new Object3D("OBJ_FROM_FILE");

		fread(buffer, 1, 4, file);
		int len = *(int*)buffer;
		fread(buffer, 1, len, file);

		// get name
		buffer[len] = '\0';
		object->SetName((char*)buffer);

		fread(buffer, 1, 8, file);
		int noofverts = *(int*)buffer;
		int noofpolys = *(int*)(buffer + 4);

		int reqsize = noofverts * 6;
		if ((config & 1) == 1)
			reqsize += noofverts * 2; // uv coordindates
		reqsize *= 4;

		// set vertices
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


		// set polygons
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

		// save object in model
		model->_model->AddEntity(object);
	}

	// save the objects to the obj vector
	free(buffer);
}

void ModelLoader::_ReadOBJ(std::string filename, ModelLoader* model) {

	// get model name
	std::vector<std::string> modelName = _GetTokens(filename, '.');
	model->_model = new Model(modelName[0]);


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

	std::map<std::string, int> globalToLocalVerts;
	std::map<int, int> globalToLocalNormals;
	std::map<int, int> globalToLocalUVs;

	std::vector<std::tuple<std::string, std::string>> relations;
	std::map<std::string, Object3D*> objectMap;

	if (file.is_open())
	{
		while (std::getline(file, line)) {

			// get tokens
			tokens = _GetTokens(line, ' ');
			if (tokens.size() > 0) {

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

						int vertIndex = stoi(subTokens[0]) - 1;
						int normalIndex = stoi(subTokens[2]) - 1;

						// has vertex been located before
						if (globalToLocalVerts.count(subTokens[0]) == 0) {

							globalToLocalVerts[subTokens[0]] = globalToLocalVerts.size();

							// make point
							PointUV p;
							int vertIndex = stoi(subTokens[0]) - 1;

							p.vertex = vertices[vertIndex];
							p.normal = normals[normalIndex];


							points.AddPoint(p);
						}
						else {

							// to convert from polygon to vertex normals, stack normals and normalise at end
							points.GetPointUV(globalToLocalVerts[subTokens[0]])->normal =
								points.GetPointUV(globalToLocalVerts[subTokens[0]])->normal + normals[normalIndex];
						}
					}

					// construct polygons
					for (int polygonID = 0; polygonID < numOfPolygons; polygonID++) {

						Poly p;

						// get global point ids convert global to local verts
						p.point[0] = globalToLocalVerts[_GetTokens(tokens[1], '/')[0]];
						p.point[1] = globalToLocalVerts[_GetTokens(tokens[polygonID + 2], '/')[0]];
						p.point[2] = globalToLocalVerts[_GetTokens(tokens[polygonID + 3], '/')[0]];

						polygons.AddPolygon(p);
					}
				}

				// object name
				if (tokens[0].compare("o") == 0) {

					// save current object
					if (currentObj.compare("") != 0) {

						// normalise normals
						for (int x = 0; x < points.Size(); x++)
							points.GetPointUV(x)->normal = glm::normalize(points.GetPointUV(x)->normal);

						// create object
						Object3D* newobj = new Object3D(currentObj);

						// assign point and polygon data
						newobj->SetVertexData(points);
						newobj->SetTriangles(polygons);

						// save object
						// model->_objects.push_back(newobj);
						model->_model->AddEntity(newobj);
						objectMap[newobj->GetName()] = newobj;

						//clear old datastructures
						points = PointData();
						points.SetUV(true);

						polygons = PolygonData();
						globalToLocalVerts.clear();
					}

					currentObj = tokens[1];
				}

				// object relations
				if (tokens[0].compare("rel") == 0) {

					relations.push_back(std::make_tuple(tokens[1], tokens[2]));
				}
			}
		}

		// ==========================================================
		// end of file

		// save last object

		// create object
		Object3D* newobj = new Object3D(currentObj);

		// assign point and polygon data
		newobj->SetVertexData(points);
		newobj->SetTriangles(polygons);

		// save object
		// model->_objects.push_back(newobj);
		model->_model->AddEntity(newobj);
		objectMap[newobj->GetName()] = newobj;

		// construct object heirarchy if defined
		Object3D* parent;
		Object3D* child;
		for (auto relation : relations) {

			parent = objectMap[std::get<0>(relation)];
			child = objectMap[std::get<1>(relation)];

			if (parent != nullptr && child != nullptr)
				parent->AddChild(child);
		}

		file.close();
	}
}

std::vector<std::string> ModelLoader::_GetTokens(std::string line, char seperator) {

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

Model* ModelLoader::GetModel() {

	return _model;
}