#include "Object3D.h"

Object3D::Object3D()
{
	vbos = NULL;
	// incuv = false;
	// vertexdata = NULL;
	//polygons = NULL;
	SetName("NULL");

	//elementcount = 0;
	//elementtype = GL_TRIANGLES;

	texturemap = -1;
}

Object3D::Object3D(Object3D* copy, std::string newName) 
	: Material((Material)copy)
	, Moveable((Moveable)copy)
{

	vbos = NULL;
	// incuv = copy->incuv;
	SetName((char*)newName.c_str());

	// assign new vertex data
	// SetVertexData(copy->vertexdata, copy->noofverts, copy->incuv);
	vertices.SetData(copy->vertices);

	// assign new polygon data
	// SetTriangles(copy->polygons, copy->elementcount);
	polygons.SetData(copy->polygons);

	texturemap = copy->texturemap;
	hasTexture = copy->hasTexture;
	textureID = copy->textureID;

	useLight = copy->useLight;
	useTexture = copy->useTexture;
}

Object3D::~Object3D()
{
	free(vbos);
	// free(vertexdata);
	// free(polygons);
	free(name);
}

// ======================================

// Draw the object using the given rendering contect
void Object3D::Draw(RenderingContext rcontext) {
	
	_AssignHandleInformation(rcontext);

	_HandleVBOs(rcontext);

	// draw the children objects
	for (Object3D* child : children)
		child->Draw(rcontext);

	// remove rotation added in _AssignHandleInformation
	rcontext.PopModelMatrix();
}

// Initialse object VBOs
void Object3D::_InitVBOs()
{
	if (!vbos)
		vbos = (unsigned int*)malloc(2 * sizeof(unsigned int));
	glGenBuffers(2, vbos);

	// int size = 4 * noofverts*(incuv ? 8 : 6);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.VectorDataSize(),
		vertices.GetVectorData(),
		GL_STATIC_DRAW
	);

	// int size = 2 * elementcount;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		polygons.VectorDataSize(),
		polygons.GetVectorData(),
		GL_STATIC_DRAW
	);
}

// Pass current object amd context information to the shaders 
void Object3D::_AssignHandleInformation(RenderingContext& rcontext) {

	// Material
	glUniform4fv(rcontext.mathandles[0], 1, ambient.rgba);
	glUniform4fv(rcontext.mathandles[1], 1, diffuse.rgba);
	glUniform4fv(rcontext.mathandles[2], 1, specular.rgba);
	glUniform1f(rcontext.mathandles[3], glossiness);

	// flags
	glUniform1i(rcontext.useslighthandle, useLight);
	glUniform1i(rcontext.usestexturehandle, useTexture);
	glUniform1i(rcontext.usesinstancing, isInstanced);
	glUniform1i(rcontext.usesbillboarding, isBillboarded);

	// bind info in textureID to the textureHandle
	if (hasTexture) {

		glEnable(GL_TEXTURE_2D);

		glUniform1i(rcontext.texturehandle, 0);
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);
	}

	if (!vbos)
		_InitVBOs();

	// save current model matrix
	rcontext.PushModelMatrix();

	// as the current translation values are global and not local, preform the offset values instead
	rcontext.Translate(localPos[0], localPos[1], localPos[2]);

	rcontext.Translate(translation[0], translation[1], translation[2]);

	rcontext.Scale(scale[0], scale[1], scale[2]);

	// preform the rotation
	rcontext.RotateX(rotation[0]);
	rcontext.RotateY(rotation[1]);
	rcontext.RotateZ(rotation[2]);


	rcontext.UpdateMVPs();
	glUniformMatrix4fv(rcontext.nmathandle, 1, false, glm::value_ptr(rcontext.normalmatrix));
	glUniformMatrix4fv(rcontext.mvphandle, 1, false, glm::value_ptr(rcontext.mvpmatrix));
	glUniformMatrix4fv(rcontext.vphandle, 1, false, glm::value_ptr(rcontext.vpmatrix));
}

// Handle VBO information for drawing
void Object3D::_HandleVBOs(RenderingContext& rcontext) {

	_HandleVertVBO(rcontext);
	_HandlePolyVBO(rcontext);
}

// Handle the vertex VBO data layout 
void Object3D::_HandleVertVBO(RenderingContext& rcontext) {

	//glVertexAttribPointer(rcontext.verthandles[0], 3, GL_FLOAT, false, 4 * 8, (void*)0);
	//glVertexAttribPointer(rcontext.verthandles[1], 3, GL_FLOAT, false, 4 * 8, (void*)(4 * 3));
	//glVertexAttribPointer(rcontext.verthandles[2], 2, GL_FLOAT, false, 4 * 8, (void*)(4 * 6));

	//glVertexAttribPointer(rcontext.verthandles[0], 3, GL_FLOAT, false, 4 * 6, (void*)0);
	//glVertexAttribPointer(rcontext.verthandles[1], 3, GL_FLOAT, false, 4 * 6, (void*)(4 * 3));

	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

	// attributes
	if (vertices.HasUV())
	{
		glVertexAttribPointer(rcontext.verthandles[0], 3, GL_FLOAT, false, sizeof(PointUV), (void*)0);
		glVertexAttribPointer(rcontext.verthandles[1], 3, GL_FLOAT, false, sizeof(PointUV), (void*)offsetof(PointUV, normal));
		glVertexAttribPointer(rcontext.verthandles[2], 2, GL_FLOAT, false, sizeof(PointUV), (void*)offsetof(PointUV, uv));
		glEnableVertexAttribArray(rcontext.verthandles[0]);
		glEnableVertexAttribArray(rcontext.verthandles[1]);
		glEnableVertexAttribArray(rcontext.verthandles[2]);
	}
	else
	{
		glVertexAttribPointer(rcontext.verthandles[0], 3, GL_FLOAT, false, sizeof(Point), (void*)0);
		glVertexAttribPointer(rcontext.verthandles[1], 3, GL_FLOAT, false, sizeof(Point), (void*)offsetof(Point, normal));
		glEnableVertexAttribArray(rcontext.verthandles[0]);
		glEnableVertexAttribArray(rcontext.verthandles[1]);
		glDisableVertexAttribArray(rcontext.verthandles[2]);
	}
}

// handle and draw the polygon VBO data
void Object3D::_HandlePolyVBO(RenderingContext& rcontext) {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	glDrawElements(
		polygons.ElementType(),
		polygons.ElementCount(),
		GL_UNSIGNED_SHORT, 
		0
	);
}

// ===================================================

// add a child to the object heirarchy
void Object3D::AddChild(Object3D* newChild) {

	children.push_back(newChild);

	glm::vec3 childPosition = newChild->GetWorldPosVec();

	float dX = childPosition.x - worldPos[0];
	float dY = childPosition.y - worldPos[1];
	float dZ = childPosition.z - worldPos[2];

	newChild->SetLocalPos(dX, dY, dZ);
};

// ======================================

// Assign vertex data to the Object
void Object3D::SetVertexData(byte* buffer, int noofverts, int bufferlen)
{
	//incuv = (bufferlen == 4 * (noofverts * 8));
	//vertexdata = (float*)malloc(bufferlen);
	//memcpy(vertexdata, buffer, bufferlen);
	//this->noofverts = noofverts;
	vertices.SetData(buffer, noofverts, bufferlen);
}

// Assign vertex data to the Object
void Object3D::SetVertexData(float* newVertData, int noofverts, bool uv) {

	//incuv = uv;
	//int length = sizeof(float) * noofverts * (uv ? 8 : 6);

	//free(vertexdata);
	//vertexdata = (float*)malloc(length);
	//memcpy(vertexdata, newVertData, length);
	//this->noofverts = noofverts;
	vertices.SetData(newVertData, noofverts, uv);
}


// ======================================

// Assign polygon data to the Object
void Object3D::SetTriangles(byte* buffer, int noofpolys) {

	//const int size = 3 * noofpolys * sizeof(unsigned short);
	//polygons = (unsigned short*)malloc(size);
	//memcpy(polygons, buffer, size);

	//elementtype = GL_TRIANGLES;
	//elementcount = 3 * noofpolys;
	polygons.SetData(buffer, noofpolys);
}

// Assign polygon data to the Object
void Object3D::SetTriangles(unsigned short* newPolyData, int noofElements) {

	//const int size = noofElements * sizeof(unsigned short);

	//free(polygons);
	//polygons = (unsigned short*)malloc(size);
	//memcpy(polygons, newPolyData, size);

	//elementtype = GL_TRIANGLES;
	//elementcount = noofElements;
	polygons.SetData(newPolyData, noofElements);
}


// ======================================

// Assign texture handle
void Object3D::SetTexture(unsigned int newTextureID) {

	hasTexture = true;
	useTexture = true;
	textureID = newTextureID;
}