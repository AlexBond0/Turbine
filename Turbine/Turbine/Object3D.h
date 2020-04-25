#pragma once
#include "GLSetup.h"
#include "RenderingContext.h"
#include "Material.h"
#include "Entity.h"
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
	public Entity
{
public:

	// ===================================================================
	// Constructors and Destructors

	Object3D(std::string name);
	Object3D(Object3D* copy, std::string newName);
	virtual ~Object3D();


	// ===================================================================
	// Render flags

	bool useLight = true;			// does the object use light information when shading
	bool useTexture = false;		// does the object use texture information when shading
	bool isActive = true;			// does the object get rendered
	bool isTransparent = false;		// does the object have transparency


	// ===================================================================
	// Implemented for Entity class

	void OnRender(RenderingContext& rcontext);
	Object3D* OnPick();


	// ===================================================================
	// Methods

	virtual void Draw(RenderingContext& rcontext);

	void SetVertexData(byte* buffer, int noofverts, int bufferlen);
	void SetVertexData(float* newVertData, int noofverts, bool uv);
	void SetVertexData(PointData data);

	void* GetVertData();
	int GetVertCount();

	void SetTriangles(byte* buffer, int noofpolys);
	void SetTriangles(unsigned short* polygons, int noofElements);
	void SetTriangles(PolygonData data);

	void SetTextureMap(int id);
	void SetTexture(unsigned int newTextureID);

	bool IsInstanced();

	PointData vertices;			// vertex data
	PolygonData polygons;		// polygon data

protected:

	// char* name;					// object name
	Object3DHandles handles;	// object handles

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
	void _BlendDraw(RenderingContext& rcontext);
};

// Set the texturemap of the object via texturemap ID
inline void Object3D::SetTextureMap(int id)
{
	texturemap = id;
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

inline void Object3D::SetVertexData(PointData data) {

	vertices.SetData(data);
}

// Assign polygon data to the Object
inline void Object3D::SetTriangles(byte* buffer, int noofpolys) {

	polygons.SetData(buffer, noofpolys);
}

// Assign polygon data to the Object
inline void Object3D::SetTriangles(unsigned short* newPolyData, int noofElements) {

	polygons.SetData(newPolyData, noofElements);
}

inline void Object3D::SetTriangles(PolygonData data) {

	polygons.SetData(data);
}

inline bool Object3D::IsInstanced() {

	return isInstanced;
}