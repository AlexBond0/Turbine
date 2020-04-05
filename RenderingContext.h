#pragma once
#include <assert.h>
#include "Matrix.h"

#define MAX_DEPTH   (32*16)


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

	float* GetCurrentModelMatrix();

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
	float viewmatrix[16], projectionmatrix[16], normalmatrix[16];

	// combined matricies
	float mvmatrix[16], mvpmatrix[16], vpmatrix[16];


private:
  int depth;
  float modelmatrix[MAX_DEPTH];

};

inline RenderingContext::RenderingContext()
{
  depth=0;
  MyMatrix::SetIdentity(viewmatrix);
  MyMatrix::SetIdentity(projectionmatrix);
  MyMatrix::SetIdentity(normalmatrix);
}  

inline void RenderingContext::InitModelMatrix(bool resetdepth)
{
  if (resetdepth)
    depth=0;
  MyMatrix::SetIdentity(modelmatrix);  
}

inline void RenderingContext::PushModelMatrix()
{
  memcpy(modelmatrix+depth+16, modelmatrix+depth, sizeof(float)*16);
  depth+=16;
}

inline void RenderingContext::PopModelMatrix()
{
  assert(depth>0);
  depth-=16;
}


// This method is done for you
inline void RenderingContext::UpdateMVPs()
{
  MyMatrix::MultiplyMatrix(mvmatrix, viewmatrix, modelmatrix+depth);
  MyMatrix::MultiplyMatrix(mvpmatrix, projectionmatrix, mvmatrix);
  MyMatrix::MultiplyMatrix(vpmatrix, projectionmatrix, viewmatrix);
  MyMatrix::CreateNormalMatrix(modelmatrix+depth, normalmatrix);
}

inline void RenderingContext::Translate(const float x, const float y, const float z)
{
  MyMatrix::Translate(modelmatrix+depth, x, y, z);
}

inline void RenderingContext::Scale(const float x, const float y, const float z)
{
  MyMatrix::Scale(modelmatrix+depth, x, y, z);
}

inline void RenderingContext::RotateX(const float degs)
{
  MyMatrix::RotateX(modelmatrix+depth, degs);
}

inline void RenderingContext::RotateY(const float degs)
{
  MyMatrix::RotateY(modelmatrix+depth, degs);
}

inline void RenderingContext::RotateZ(const float degs)
{
  MyMatrix::RotateZ(modelmatrix+depth, degs);
}

inline float* RenderingContext::GetCurrentModelMatrix() {

	return (modelmatrix + depth);
}