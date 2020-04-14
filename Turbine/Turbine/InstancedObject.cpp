#include "InstancedObject.h"



InstancedObject::InstancedObject() {

	isInstanced = true;

	// _GenerateSomeParticles();
}

InstancedObject::InstancedObject(Object3D* copy, std::string newName) 
	: Object3D(copy, newName)
{

	isInstanced = true;
}


InstancedObject::~InstancedObject() {

	free(instanceData);
}

// overriden function from Object3D so instance VBO is created correctly
void InstancedObject::_InitVBOs() {

	if (!vbos)
		vbos = (unsigned int*)malloc(3 * sizeof(unsigned int));
	glGenBuffers(3, vbos);

	// vertexes
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.VectorDataSize(),
		vertices.GetVectorData(),
		GL_STATIC_DRAW
	);

	// polygons
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		polygons.DataSize(),
		polygons.GetData(),
		GL_STATIC_DRAW
	);

	// instances
	int size = 4 * 3 * noofinstances;
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, size, instanceData, GL_DYNAMIC_DRAW);
}

// overriden function from Object3D so instance VBO is handled correctly
void InstancedObject::_HandleVBOs(RenderingContext& rcontext) {

	// select the instance VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);

	// pass instance data to the shader
	glVertexAttribPointer(rcontext.instancehandle, 3, GL_FLOAT, false, (4 * 3), (void*)0);
	glEnableVertexAttribArray(rcontext.instancehandle);

	Object3D::_HandleVertVBO(rcontext);

	// tell the polygon data to handle instancing correctly
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	glVertexAttribDivisor(rcontext.instancehandle, 1);
	glDrawElementsInstanced(
		polygons.ElementType(),
		polygons.ElementCount(),
		GL_UNSIGNED_SHORT,
		0,
		noofinstances
	);
}

// =====================================================================

// set instance data for this object
void InstancedObject::SetInstanceData(float* newInstanceData, int noofnewinstances) {

	int length = sizeof(float) * noofnewinstances * 3;

	free(instanceData);
	instanceData = (float*)malloc(length);
	memcpy(instanceData, newInstanceData, length);
	noofinstances = noofnewinstances;

	if (vbos && noofinstances > 0) {

		glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
		glBufferData(GL_ARRAY_BUFFER, length, instanceData, GL_DYNAMIC_DRAW);
	}
}