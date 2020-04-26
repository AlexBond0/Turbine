#pragma once
#include <assert.h>
#include "Matrix.h"
#include "Shader.h"

// #define MAX_DEPTH   (32*16)
#define MAX_DEPTH   32
#define DEGS_TO_RADS(x) x*(3.141592f/180.0f)

class TranslationStack {

public:

	TranslationStack();

	void InitModelMatrix(bool resetdepth);
	void PushModelMatrix();
	void PopModelMatrix();

	void UpdateMVPs();

	// These all assume the transformation happens to the top-most matrix in the stack (model view)
	void Translate(const float x, const float y, const float z);
	void Scale(const float x, const float y, const float z);
	void RotateX(const float degs);
	void RotateY(const float degs);
	void RotateZ(const float degs);
	void Rotate(glm::mat4 rotationMat);

	glm::mat4* GetCurrentModelMatrix();

	// raw matricies
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 normalMatrix;

	// combined matricies
	//glm::mat4 mvmatrix;
	//glm::mat4 mvpmatrix;
	//glm::mat4 vpmatrix;

private:

	int _depth;
	glm::mat4 _modelmatrix[MAX_DEPTH];
};

// Type of render currently being preformed
enum class RenderPass {
	OBJECT,	// solid objects rendering
	BLEND	// transparent objects rendering
};

class RenderingContext
 : public TranslationStack {

public:

	RenderingContext();
	~RenderingContext();

	std::map<std::string, Shader*> shaders;

	bool createdShaders = false;	// has the renderingcontext generated valid openGL shanders & handles
	RenderPass renderPass;			// current type of rendering pass being preformed
};