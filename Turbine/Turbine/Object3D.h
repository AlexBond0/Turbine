#pragma once
#include "GLSetup.h"
#include "RenderingContext.h"
#include "Material.h"
#include "Moveable.h"
#include "ObjectDataHandlers.h"

#include <vector>
#include <map>


// Defines an object in the scene, along with handeling its rendering 
class Object3D :
	public Material,
	public Moveable
{
public:

	bool useLight = true;		// does the object use light information when shading
	bool useTexture = false;	// does the object use texture information when shading

	Object3D();
	Object3D(Object3D* copy, std::string newName);
	virtual ~Object3D();

	virtual void Draw(RenderingContext rcontext);

	char* GetName();
	void SetName(const char* name);

	void SetVertexData(byte* buffer, int noofverts, int bufferlen);
	void SetVertexData(float* newVertData, int noofverts, bool uv);

	float* GetVertData();
	int GetVertCount();

	void SetTriangles(byte* buffer, int noofpolys);
	void SetTriangles(unsigned short* polygons, int noofElements);

	void SetTextureMap(int id);
	void SetTexture(unsigned int newTextureID);

	std::vector<Object3D*> GetChildren();
	void AddChild(Object3D* newChild);

protected:

	char* name;	// object name

	std::vector<Object3D*> children; // children in object heirarchy

	// bool incuv;	// include uv values when sending to shader

	// vertex data
	//float* vertexdata;
	//int noofverts;
	PointData vertices;

	// polygon data
	unsigned short* polygons;
	int elementtype;
	int elementcount;

	// texture data
	bool hasTexture = false;
	unsigned int textureID;

	bool isInstanced = false;
	bool isBillboarded = false;

	// data vbos
	unsigned int* vbos;

	int texturemap;

	virtual void _InitVBOs();

	virtual void _AssignHandleInformation(RenderingContext& rcontext);

	virtual void _HandleVBOs(RenderingContext& rcontext);
	virtual void _HandleVertVBO(RenderingContext& rcontext);
	virtual void _HandlePolyVBO(RenderingContext& rcontext);

};

inline void Object3D::SetTextureMap(int id)
{
	texturemap = id;
}

inline void Object3D::SetName(const char* name)
{
	const int len = strlen(name) + 1;
	this->name = (char*)realloc(this->name, len);
	strcpy_s(this->name, len, name);
}

inline char* Object3D::GetName() {

	return name;
}

inline float* Object3D::GetVertData() {

	return vertices.GetData(); // vertexdata;
}

inline int Object3D::GetVertCount() {

	return vertices.Size(); //noofverts;
}

inline std::vector<Object3D*> Object3D::GetChildren() {

	return children;
}