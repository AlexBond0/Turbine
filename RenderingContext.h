#pragma once
#include <assert.h>
#include "Matrix.h"

// #define MAX_DEPTH   (32*16)
#define MAX_DEPTH   32
#define DEGS_TO_RADS(x) x*(3.141592f/180.0f)

class RenderingContext
{
public:
	RenderingContext();

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

	glm::mat4* GetCurrentModelMatrix();

	// ====================================================
	//			--= HANDLES =--

	// shaders & program
	int glprogram;
	int nmathandle, mvphandle, vphandle;

	// if you are reading this, feel free to uncomment the following two lines and use these declarations for the light and material handles
	int lighthandles[5];  // direction; halfplane; ambient; diffuse; specular
	int mathandles[4];    // ambient, diffuse, specular, shininess
	int verthandles[2];   // position, normal, UV

	// texturing
	int texturehandle;
	int useslighthandle;
	int usestexturehandle;

	// instancing
	int usesinstancing;
	int instancehandle;

	// billboarding
	int usesbillboarding;
	int billboardhandles[2];	// up, right

	// ====================================================
	//			--= MATRICIES =--

	// raw matricies
	glm::mat4 viewmatrix;
	glm::mat4 projectionmatrix;
	glm::mat4 normalmatrix;

	// combined matricies
	glm::mat4 mvmatrix;;
	glm::mat4 mvpmatrix; 
	glm::mat4 vpmatrix;


private:

  int depth;
  glm::mat4 modelmatrix[MAX_DEPTH];

};

inline RenderingContext::RenderingContext() {

  depth = 0;
  viewmatrix = glm::mat4(1.0);
  projectionmatrix = glm::mat4(1.0);
  normalmatrix = glm::mat4(1.0);

  /*MyMatrix::SetIdentity(viewmatrix);
  MyMatrix::SetIdentity(projectionmatrix);
  MyMatrix::SetIdentity(normalmatrix);*/
}  

inline void RenderingContext::InitModelMatrix(bool resetdepth) {

	if (resetdepth)
		depth = 0;

	modelmatrix[depth] = glm::mat4(1.0);

	// MyMatrix::SetIdentity(modelmatrix);  
}

inline void RenderingContext::PushModelMatrix() {

	modelmatrix[depth + 1] = glm::mat4(modelmatrix[depth]);
	depth++;

	// memcpy(modelmatrix+depth+16, modelmatrix+depth, sizeof(float)*16);
	// depth+=16;
}

inline void RenderingContext::PopModelMatrix() {

	assert(depth > 0);
	depth--;

	//assert(depth>0);
	//depth-=16;
}


// This method is done for you
inline void RenderingContext::UpdateMVPs() {

	mvmatrix = viewmatrix * modelmatrix[depth];
	mvpmatrix = projectionmatrix * mvmatrix;
	vpmatrix = projectionmatrix * viewmatrix;

	// swap to mv matrix to have lighing  info follow view matrix too
	normalmatrix = glm::transpose(glm::inverse(modelmatrix[depth]));

  //MyMatrix::MultiplyMatrix(mvmatrix, viewmatrix, modelmatrix+depth);
  //MyMatrix::MultiplyMatrix(mvpmatrix, projectionmatrix, mvmatrix);
  //MyMatrix::MultiplyMatrix(vpmatrix, projectionmatrix, viewmatrix);
  //MyMatrix::CreateNormalMatrix(modelmatrix+depth, normalmatrix);
}

inline void RenderingContext::Translate(const float x, const float y, const float z) {

	modelmatrix[depth] = glm::translate(modelmatrix[depth], glm::vec3(x, y, z));

	// MyMatrix::Translate(modelmatrix+depth, x, y, z);
}

inline void RenderingContext::Scale(const float x, const float y, const float z) {

	modelmatrix[depth] = glm::scale(modelmatrix[depth], glm::vec3(x, y, z));
	// MyMatrix::Scale(modelmatrix+depth, x, y, z);
}

inline void RenderingContext::RotateX(const float degs) {

	modelmatrix[depth] = glm::rotate(modelmatrix[depth], DEGS_TO_RADS(degs), glm::vec3(1, 0, 0));
	// MyMatrix::RotateX(modelmatrix+depth, degs);
}

inline void RenderingContext::RotateY(const float degs) {

	modelmatrix[depth] = glm::rotate(modelmatrix[depth], DEGS_TO_RADS(degs), glm::vec3(0, 1, 0));
	// MyMatrix::RotateY(modelmatrix+depth, degs);
}

inline void RenderingContext::RotateZ(const float degs) {

	modelmatrix[depth] = glm::rotate(modelmatrix[depth], DEGS_TO_RADS(degs), glm::vec3(0, 0, 1));
	// MyMatrix::RotateZ(modelmatrix+depth, degs);
}

inline glm::mat4* RenderingContext::GetCurrentModelMatrix() {

	return &modelmatrix[depth];
}