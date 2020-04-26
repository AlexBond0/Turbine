#include "TranslationStack.h"

// Initialise the rendering context 
TranslationStack::TranslationStack() {

	_depth = 0;
	viewMatrix = glm::mat4(1.0);
	projectionMatrix = glm::mat4(1.0);
	normalMatrix = glm::mat4(1.0);
}

// Initialise the model matrix, and optionally reset the _depth to 0
void TranslationStack::InitModelMatrix(bool reset_depth) {

	if (reset_depth)
		_depth = 0;

	_modelmatrix[_depth] = glm::mat4(1.0);
}

// push the current model matrix to the matrix stack
void TranslationStack::PushModelMatrix() {

	_modelmatrix[_depth + 1] = glm::mat4(_modelmatrix[_depth]);
	_depth++;
}

// pop the current model matrix off of the stack
void TranslationStack::PopModelMatrix() {

	assert(_depth > 0);
	_depth--;
}


// Update model, view and projection matrices
void TranslationStack::UpdateMVPs() {

	// swap to mv matrix to have lighing  info follow view matrix too
	normalMatrix = glm::transpose(glm::inverse(_modelmatrix[_depth]));
}

// Translate the current model matrix
void TranslationStack::Translate(const float x, const float y, const float z) {

	_modelmatrix[_depth] = glm::translate(_modelmatrix[_depth], glm::vec3(x, y, z));
}

// Scale the current model matrix
void TranslationStack::Scale(const float x, const float y, const float z) {

	_modelmatrix[_depth] = glm::scale(_modelmatrix[_depth], glm::vec3(x, y, z));
}

// Rotate the current model matrix around the X axis
void TranslationStack::RotateX(const float degs) {

	_modelmatrix[_depth] = glm::rotate(_modelmatrix[_depth], DEGS_TO_RADS(degs), glm::vec3(1, 0, 0));
}

// Rotate the current model matrix around the Y axis
void TranslationStack::RotateY(const float degs) {

	_modelmatrix[_depth] = glm::rotate(_modelmatrix[_depth], DEGS_TO_RADS(degs), glm::vec3(0, 1, 0));
}

// Rotate the current model matrix around the Z axis
void TranslationStack::RotateZ(const float degs) {

	_modelmatrix[_depth] = glm::rotate(_modelmatrix[_depth], DEGS_TO_RADS(degs), glm::vec3(0, 0, 1));
}

void TranslationStack::Rotate(glm::mat4 rotationMat) {

	_modelmatrix[_depth] *= rotationMat;
}

// Get a pointer to the current model matrix
glm::mat4* TranslationStack::GetCurrentModelMatrix() {

	return &_modelmatrix[_depth];
}