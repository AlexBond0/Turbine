#include "InstancedObject.h"



InstancedObject::InstancedObject(std::string name)
	: Object3D(name)
{
	SetEntityType(EntityType::OBJ_INSTANCED);
	isInstanced = true;
}

InstancedObject::InstancedObject(Object3D* copy, std::string newName) 
	: Object3D(copy, newName)
{
	SetEntityType(EntityType::OBJ_INSTANCED);
	isInstanced = true;
}


InstancedObject::~InstancedObject() {

	// delete instance handle, the other handles are managed by the base Object3D class
	if (handles.initialised)
		glDeleteBuffers(1, &handles.instance_vbo);

	// free(instanceData);
}

// Bind the VAO and draw the object
void InstancedObject::_Draw(RenderingContext& rcontext) {

	glBindVertexArray(handles.object_vao);

	// tell the polygon data to handle instancing correctly
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles.polygon_vbo);
	glVertexAttribDivisor(3, 1);
	glDrawElementsInstanced(
		polygons.ElementType(),
		polygons.ElementCount(),
		GL_UNSIGNED_SHORT,
		0,
		_noofinstances
	);

	glBindVertexArray(0);
}

// overriden function from Object3D so instance VBO is created correctly
void InstancedObject::_InitVBOs() {

	// generate VAO
	glGenVertexArrays(1, &handles.object_vao);
	glBindVertexArray(handles.object_vao);

	// generate VBOs
	glGenBuffers(1, &handles.point_vbo);
	glGenBuffers(1, &handles.polygon_vbo);
	glGenBuffers(1, &handles.instance_vbo);

	// vertexes
	glBindBuffer(GL_ARRAY_BUFFER, handles.point_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.DataSize(),
		vertices.GetData(),
		GL_STATIC_DRAW
	);

	// polygons
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handles.polygon_vbo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		polygons.DataSize(),
		polygons.GetData(),
		GL_STATIC_DRAW
	);

	// instances
	// int size = 4 * 3 * noofinstances;
	if (instances.Size() > 0) {

		glBindBuffer(GL_ARRAY_BUFFER, handles.instance_vbo);
		glBufferData(
			GL_ARRAY_BUFFER,
			InstanceDataSize(),
			instances.GetData(),
			GL_DYNAMIC_DRAW
		);
	}

	handles.initialised = true;

	glBindVertexArray(0);
}

// overriden function from Object3D so instance VBO is handled correctly
void InstancedObject::_HandleVBOs(RenderingContext& rcontext) {

	if (handles.dirty) {

		glBindVertexArray(handles.object_vao);

		// select the instance VBO
		glBindBuffer(GL_ARRAY_BUFFER, handles.instance_vbo);

		// pass instance data to the shader || rcontext.instancehandle
		glVertexAttribPointer(3, 3, GL_FLOAT, false, (4 * 3), (void*)0);
		glEnableVertexAttribArray(3);

		glBindVertexArray(0);


		Object3D::_HandleVertVBO(rcontext);

		handles.dirty = false;
	}
}

// Update number of instaces to render
// This is seperate to number of instances stored as not all instances stored may want to be rendered
void InstancedObject::UpdateInstanceCount() {

	_noofinstances = instances.Size();

	UpdateInstanceCount(_noofinstances);
}

// Update number of instaces to render with a given instance count
// This is seperate to number of instances stored as not all instances stored may want to be rendered
void InstancedObject::UpdateInstanceCount(int instanceCount) {

	if (instanceCount < 0)
		_noofinstances = instances.Size();
			
	else
		_noofinstances = instanceCount;

	if (handles.initialised && _noofinstances > 0) {
		
		glBindBuffer(GL_ARRAY_BUFFER, handles.instance_vbo);
		glBufferData(
			GL_ARRAY_BUFFER, 
			InstanceDataSize(),
			instances.GetData(), 
			GL_DYNAMIC_DRAW
		);
	}
}

int InstancedObject::GetInstanceCount() {
	
	return _noofinstances;
}

int InstancedObject::InstanceDataSize() {

	return sizeof(Instance) * GetInstanceCount();
}