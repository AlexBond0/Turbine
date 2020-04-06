#pragma once
#include <math.h> // remember to add _USE_MATH_DEFINES to the project settings
#include <memory.h>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#define DEGSTORADS(x)     ((x*M_PI)/180.0f)

/*
The matrix is assumed to be stored as illustrated below:
m[0] m[4] m[8]  m[12]
m[1] m[5] m[9]  m[13]
m[2] m[6] m[10] m[14]
m[3] m[7] m[11] m[15]
*/

class MyMatrix
{
public:
	static void SetIdentity(float* matrix);
	static void Normalise3(float* vector);

	// static void CrossProduct3(const float* vec1, const float* vec2, float* dst);
	static float DotProduct3(const float* vec1, const float* vec2);

	static void MultiplyMatrix(float* result, const float* lhs, const float* rhs);
	static void Translate(float* matrix, const float tx, const float ty, const float tz);

	static void Scale(float* matrix, const float sx, const float sy, const float sz);
	static void Scale(float* vec3, const float scale);

	static void RotateX(float* matrix, const float degs);
	static void RotateY(float* matrix, const float degs);
	static void RotateZ(float* matrix, const float degs);
	static void RotateXYZ(float* matrix, const float degsX, const float degsY, const float degsZ);

	static void MultiplyVecByMatrix(float* vec3, const float* mat3);
	static void RotateMat4ByVec3(float* mat4, const float* vec3, const float rotation);

	static void InvertMat4(const float* m, float* invOut);
	static void TransposeMat4(const float* m, float* transpose);
	static void CreateNormalMatrix(const float* model, float* normal);

	static float length(float* vec3);
	static float distance(float* veca, float* vecb);

	static void SetOrtho(float* matrix, const float left, const float right, const float bottom, const float top, const float znear, const float zfar);
	static void SetFrustum(float* matrix, const float left, const float right, const float bottom, const float top, const float znear, const float zfar);
	
	// static void SetLookAt(float* matrix, const float* eye, const float* centre, const float* up);
	// static void SetLookAtGLM(float* matrix, const glm::vec3 eye, const glm::vec3 centre, const glm::vec3 up);
};
