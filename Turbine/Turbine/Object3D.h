#pragma once
#include "GLSetup.h"
#include "RenderingContext.h"
#include "Material.h"
#include "Moveable.h"
#include "PointData.h"
#include "PolygonData.h"

#include <vector>
#include <map>

struct Object3DHandles {

	unsigned int point_vbo;		// vbo hadle for the Object3Ds point data
	unsigned int polygon_vbo;	// vbo hanle for the Object3Ds polygon data
	unsigned int instance_vbo;	// vbo hanle for the Object3Ds instance data

	unsigned int object_vao;	// vao handle for the Object3D

	bool initialised = false;	// are the handles initialised
	bool dirty = true;			// do handles need reassigning
};


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

	void* GetVertData();
	int GetVertCount();

	void SetTriangles(byte* buffer, int noofpolys);
	void SetTriangles(unsigned short* polygons, int noofElements);

	void SetTextureMap(int id);
	void SetTexture(unsigned int newTextureID);

	bool IsInstanced();

	std::vector<Object3D*> GetChildren();
	void AddChild(Object3D* newChild);
	void SetParent(Object3D* newParent);

	TranslationStack GetWorldTranslation();

	PointData vertices;			// vertex data
	PolygonData polygons;		// polygon data

protected:

	char* name;					// object name
	Object3DHandles handles;	// object handles

	std::vector<Object3D*> children;	// children in object heirarchy
	Object3D* parent;					// parent in object heirarchy

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
	virtual void _Draw(RenderingContext& rcontext);

	void _GetWorldTranslation(TranslationStack& translations);
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
inline void* Object3D::GetVertData() {

	return vertices.GetData();
}

// Get amount of verticies defining the object
inline int Object3D::GetVertCount() {

	return vertices.Size();
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

inline bool Object3D::IsInstanced() {

	return isInstanced;
}