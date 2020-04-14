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

	char* name;					// object name
	unsigned int* vbos;			// data vbos

	std::vector<Object3D*> children; // children in object heirarchy

	PointData vertices;			// vertex data
	PolygonData polygons;		// polygon data

	// texture data
	bool hasTexture = false;	// does the object have a texture
	unsigned int textureID;		// textureid for the object
	int texturemap;				// texturemap for the object?

	// rendering flags
	bool isInstanced = false;	// is the object instanced
	bool isBillboarded = false;	// is the object billboarded



	virtual void _InitVBOs();

	virtual void _AssignHandleInformation(RenderingContext& rcontext);
	virtual void _HandleVBOs(RenderingContext& rcontext);
	virtual void _HandleVertVBO(RenderingContext& rcontext);
	virtual void _HandlePolyVBO(RenderingContext& rcontext);
};

// Set the texturemap of the object via texturemap ID
inline void Object3D::SetTextureMap(int id)
{
	texturemap = id;
}

// Set the name of the object
inline void Object3D::SetName(const char* name)
{
	const int len = strlen(name) + 1;
	this->name = (char*)realloc(this->name, len);
	strcpy_s(this->name, len, name);
}

// Get name of object
inline char* Object3D::GetName() {

	return name;
}

// Get Vertex Data in the form of a float* array
// NEEDS DEPRECIATING
inline float* Object3D::GetVertData() {

	return vertices.GetPointerArrayData(); // vertexdata;
}

// Get amount of verticies defining the object
inline int Object3D::GetVertCount() {

	return vertices.Size(); //noofverts;
}

// Get vector of child objects 
inline std::vector<Object3D*> Object3D::GetChildren() {

	return children;
}

// ======================================

// Assign vertex data to the Object
inline void Object3D::SetVertexData(byte* buffer, int noofverts, int bufferlen)
{
	vertices.SetData(buffer, noofverts, bufferlen);
}

// Assign vertex data to the Object
inline void Object3D::SetVertexData(float* newVertData, int noofverts, bool uv) {

	vertices.SetData(newVertData, noofverts, uv);
}

// Assign polygon data to the Object
inline void Object3D::SetTriangles(byte* buffer, int noofpolys) {

	polygons.SetData(buffer, noofpolys);
}

// Assign polygon data to the Object
inline void Object3D::SetTriangles(unsigned short* newPolyData, int noofElements) {

	polygons.SetData(newPolyData, noofElements);
}