#include "Object3D.h"

Object3D::Object3D(std::string name)
	: Entity(name, EntityType::OBJ)
{
	// SetName("NULL");
	texturemap = -1;
	parent = nullptr;
}

Object3D::Object3D(Object3D* copy, std::string newName) 
	: Material((Material)copy)
	, Entity(copy)
{

	SetName((char*)newName.c_str());

	// assign vertices and polygons
	vertices.SetData(copy->vertices);
	polygons.SetData(copy->polygons);

	texturemap = copy->texturemap;
	hasTexture = copy->hasTexture;
	textureID = copy->textureID;

	useLight = copy->useLight;
	useTexture = copy->useTexture;
}

Object3D::~Object3D()
{
	// delete handles
	if (handles.initialised) {

		glDeleteVertexArrays(1, &handles.object_vao);

		glDeleteBuffers(1, &handles.polygon_vbo);
		glDeleteBuffers(1, &handles.instance_vbo);
	}

	// free(name);
}

// ======================================

// Handle action taken by Object3D when called form entity heirarchy
void Object3D::OnRender(RenderingContext& rcontext) {

	Draw(rcontext);
}

// Handle action when sending data to the picking function
Object3D* Object3D::OnPick() {

	return this;
}

// Draw the object using the given rendering contect
void Object3D::Draw(RenderingContext& rcontext) {
	
	_AssignHandleInformation(rcontext);

	_HandleVBOs(rcontext);

	// is the object active
	if (isActive && isLocallyActive) {

		// if current render is for solid objects and obj is not transparent
		if (!isTransparent && (rcontext.renderPass == RenderPass::OBJECT)) {

			_Draw(rcontext);
		}

		// if current render is for transparency and obj is transparent
		if (isTransparent && (rcontext.renderPass == RenderPass::BLEND)) {

			_BlendDraw(rcontext);
		}

	}

	if (isActive) {

		// draw the children objects
		for (Entity* child : children) {

			child->OnRender(rcontext);
		}
	}

	// remove rotation added in _AssignHandleInformation
	rcontext.PopModelMatrix();
}


// Bind the VAO and draw the object
void Object3D::_Draw(RenderingContext& rcontext) {

	glBindVertexArray(handles.object_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles.polygon_vbo);

	glDrawElements(
		polygons.ElementType(),
		polygons.ElementCount(),
		GL_UNSIGNED_SHORT,
		0
	);
	glBindVertexArray(0);
}

// Draw the object using the given rendering contect
void Object3D::_BlendDraw(RenderingContext& rcontext) {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_DST_COLOR
	
	_Draw(rcontext);

	glDisable(GL_BLEND);
}

// Initialse object VBOs
void Object3D::_InitVBOs() {

	// generate VAO
	glGenVertexArrays(1, &handles.object_vao);
	glBindVertexArray(handles.object_vao);

	// generate VBOs
	glGenBuffers(1, &handles.point_vbo);
	glGenBuffers(1, &handles.polygon_vbo);

	// bind the verticies
	glBindBuffer(GL_ARRAY_BUFFER, handles.point_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.DataSize(),
		vertices.GetData(),
		GL_STATIC_DRAW
	);

	// bind the polygons
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles.polygon_vbo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		polygons.DataSize(),
		polygons.GetData(),
		GL_STATIC_DRAW
	);

	handles.initialised = true;

	glBindVertexArray(0);
}

// Pass current object amd context information to the shaders 
void Object3D::_AssignHandleInformation(RenderingContext& rcontext) {

	// Material
	rcontext.liveShader->SetColor("u_m_ambient", GetAmbient()->rgba);
	rcontext.liveShader->SetColor("u_m_diffuse", GetDiffuse()->rgba);
	rcontext.liveShader->SetColor("u_m_specular", GetSpecular()->rgba); //u_m_shininess
	rcontext.liveShader->SetFloat("u_m_shininess", glossiness);

	// flags
	rcontext.liveShader->SetBool("u_usesLight", useLight);
	rcontext.liveShader->SetBool("u_usesTexture", useTexture);
	rcontext.liveShader->SetBool("u_instancing", isInstanced);
	rcontext.liveShader->SetBool("u_billboarding", isBillboarded);

	// bind info in textureID to the textureHandle
	if (hasTexture) {

		glEnable(GL_TEXTURE_2D);

		// glUniform1i(rcontext.texturehandle, 0);
		rcontext.liveShader->SetInt("u_texture", 0);
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

	if (!handles.initialised)
		_InitVBOs();

	// save current model matrix
	rcontext.PushModelMatrix();

	// as the current translation values are global and not local, preform the offset values instead
	rcontext.Translate(_localPos);
	rcontext.Translate(_translation);
	rcontext.Scale(_scale);

	// preform the rotation
	rcontext.Rotate(glm::toMat4(_orientation));
	rcontext.Rotate(glm::toMat4(_rotation));


	rcontext.UpdateMVPs();
	rcontext.liveShader->SetMatrix("u_normal_matrix", rcontext.normalMatrix);
	rcontext.liveShader->SetMatrix("u_model_matrix", *rcontext.GetCurrentModelMatrix());
	rcontext.liveShader->SetMatrix("u_view_matrix", rcontext.viewMatrix);
	rcontext.liveShader->SetMatrix("u_projection_matrix", rcontext.projectionMatrix);
}

// Handle VBO information for drawing
void Object3D::_HandleVBOs(RenderingContext& rcontext) {

	if (handles.dirty) {

		_HandleVertVBO(rcontext);
		handles.dirty = false;
	}
}

// Handle the vertex VBO data layout 
void Object3D::_HandleVertVBO(RenderingContext& rcontext) {

	glBindVertexArray(handles.object_vao);
	glBindBuffer(GL_ARRAY_BUFFER, handles.point_vbo);

	// attributes
	if (vertices.HasUV())
	{
		// using 0, 1 and 2 via the layout (location = 0) definition in GLSL
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(PointUV), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(PointUV), (void*)offsetof(PointUV, normal));
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(PointUV), (void*)offsetof(PointUV, uv));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
	else
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Point), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Point), (void*)offsetof(Point, normal));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}

	glBindVertexArray(0);
}

// Assign texture handle
void Object3D::SetTexture(unsigned int newTextureID) {

	hasTexture = true;
	useTexture = true;
	textureID = newTextureID;
}