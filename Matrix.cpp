#include "Matrix.h"


//void MyMatrix::CrossProduct3(const float* vec1, const float* vec2, float* dst) {
//
//	/*dst[0] = (vec1[1] * vec2[2]) - (vec1[2] * vec2[1]);
//	dst[1] = (vec1[2] * vec2[0]) - (vec1[0] * vec2[2]);
//	dst[2] = (vec1[0] * vec2[1]) - (vec1[1] * vec2[0]);*/
//
//	glm::vec3 v1 = glm::vec3(vec1[0], vec1[1], vec1[2]);
//	glm::vec3 v2 = glm::vec3(vec2[0], vec2[1], vec2[2]);
//
//	v1 = glm::cross(v1, v2);
//
//	dst[0] = v1.x;
//	dst[1] = v1.y;
//	dst[2] = v1.z;
//}

float MyMatrix::DotProduct3(const float* vec1, const float* vec2) {

	// return (vec1[0] * vec2[0]) + (vec1[1] * vec2[1]) + (vec1[2] * vec2[2]);

	glm::vec3 v1 = glm::vec3(vec1[0], vec1[1], vec1[2]);
	glm::vec3 v2 = glm::vec3(vec2[0], vec2[1], vec2[2]);

	return glm::dot(v1, v2);
}


void MyMatrix::SetIdentity(float* matrix) {

	memset(matrix, 0, sizeof(float) * 16);
	matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1;
}

void MyMatrix::Translate(float* matrix, const float tx, const float ty, const float tz) {

	matrix[12] += matrix[0] * tx + matrix[4] * ty + matrix[8] * tz;
	matrix[13] += matrix[1] * tx + matrix[5] * ty + matrix[9] * tz;
	matrix[14] += matrix[2] * tx + matrix[6] * ty + matrix[10] * tz;
	matrix[15] += matrix[3] * tx + matrix[7] * ty + matrix[11] * tz;
}

void MyMatrix::Scale(float* matrix, const float sx, const float sy, const float sz) {

	matrix[0] *= sx;
	matrix[1] *= sy;
	matrix[2] *= sz;
	matrix[3] *= sx;
	matrix[4] *= sy;
	matrix[5] *= sz;
	matrix[6] *= sx;
	matrix[7] *= sy;
	matrix[8] *= sz;
	matrix[9] *= sx;
	matrix[10] *= sy;
	matrix[11] *= sz;
}

void MyMatrix::RotateX(float* matrix, const float degs) {

	float rads = DEGSTORADS(degs);

	float a4 = matrix[4] * cos(rads) + matrix[8] * sin(rads);
	float a8 = matrix[4] * -sin(rads) + matrix[8] * cos(rads);

	float a5 = matrix[5] * cos(rads) + matrix[9] * sin(rads);
	float a9 = matrix[5] * -sin(rads) + matrix[9] * cos(rads);

	float a6 = matrix[6] * cos(rads) + matrix[10] * sin(rads);
	float a10 = matrix[6] * -sin(rads) + matrix[10] * cos(rads);

	float a7 = matrix[7] * cos(rads) + matrix[11] * sin(rads);
	float a11 = matrix[7] * -sin(rads) + matrix[11] * cos(rads);

	matrix[4] = a4;
	matrix[5] = a5;
	matrix[6] = a6;
	matrix[7] = a7;
	matrix[8] = a8;
	matrix[9] = a9;
	matrix[10] = a10;
	matrix[11] = a11;
}

void MyMatrix::RotateY(float* matrix, const float degs) {

	float rads = DEGSTORADS(degs);

	float a0 = matrix[0] * cos(rads) + matrix[8] * -sin(rads);
	float a8 = matrix[0] * sin(rads) + matrix[8] * cos(rads);

	float a1 = matrix[1] * cos(rads) + matrix[9] * -sin(rads);
	float a9 = matrix[1] * sin(rads) + matrix[9] * cos(rads);

	float a2 = matrix[2] * cos(rads) + matrix[10] * -sin(rads);
	float a10 = matrix[2] * sin(rads) + matrix[10] * cos(rads);

	float a3 = matrix[3] * cos(rads) + matrix[11] * -sin(rads);
	float a11 = matrix[3] * sin(rads) + matrix[11] * cos(rads);

	matrix[0] = a0;
	matrix[1] = a1;
	matrix[2] = a2;
	matrix[3] = a3;
	matrix[8] = a8;
	matrix[9] = a9;
	matrix[10] = a10;
	matrix[11] = a11;
}

void MyMatrix::RotateZ(float* matrix, const float degs) {

	float rads = DEGSTORADS(degs);

	float a0 = matrix[0] * cos(rads) + matrix[4] * sin(rads);
	float a4 = matrix[0] * -sin(rads) + matrix[4] * cos(rads);

	float a1 = matrix[1] * cos(rads) + matrix[5] * sin(rads);
	float a5 = matrix[1] * -sin(rads) + matrix[5] * cos(rads);

	float a2 = matrix[2] * cos(rads) + matrix[6] * sin(rads);
	float a6 = matrix[2] * -sin(rads) + matrix[6] * cos(rads);

	float a3 = matrix[3] * cos(rads) + matrix[7] * sin(rads);
	float a7 = matrix[3] * -sin(rads) + matrix[7] * cos(rads);

	matrix[0] = a0;
	matrix[1] = a1;
	matrix[2] = a2;
	matrix[3] = a3;
	matrix[4] = a4;
	matrix[5] = a5;
	matrix[6] = a6;
	matrix[7] = a7;
}

void MyMatrix::MultiplyMatrix(float* result, const float* lhs, const float* rhs) {

	result[0] = lhs[0] * rhs[0] + lhs[4] * rhs[1] + lhs[8] * rhs[2] + lhs[12] * rhs[3];
	result[1] = lhs[1] * rhs[0] + lhs[5] * rhs[1] + lhs[9] * rhs[2] + lhs[13] * rhs[3];
	result[2] = lhs[2] * rhs[0] + lhs[6] * rhs[1] + lhs[10] * rhs[2] + lhs[14] * rhs[3];
	result[3] = lhs[3] * rhs[0] + lhs[7] * rhs[1] + lhs[11] * rhs[2] + lhs[15] * rhs[3];

	result[4] = lhs[0] * rhs[4] + lhs[4] * rhs[5] + lhs[8] * rhs[6] + lhs[12] * rhs[7];
	result[5] = lhs[1] * rhs[4] + lhs[5] * rhs[5] + lhs[9] * rhs[6] + lhs[13] * rhs[7];
	result[6] = lhs[2] * rhs[4] + lhs[6] * rhs[5] + lhs[10] * rhs[6] + lhs[14] * rhs[7];
	result[7] = lhs[3] * rhs[4] + lhs[7] * rhs[5] + lhs[11] * rhs[6] + lhs[15] * rhs[7];

	result[8] = lhs[0] * rhs[8] + lhs[4] * rhs[9] + lhs[8] * rhs[10] + lhs[12] * rhs[11];
	result[9] = lhs[1] * rhs[8] + lhs[5] * rhs[9] + lhs[9] * rhs[10] + lhs[13] * rhs[11];
	result[10] = lhs[2] * rhs[8] + lhs[6] * rhs[9] + lhs[10] * rhs[10] + lhs[14] * rhs[11];
	result[11] = lhs[3] * rhs[8] + lhs[7] * rhs[9] + lhs[11] * rhs[10] + lhs[15] * rhs[11];

	result[12] = lhs[0] * rhs[12] + lhs[4] * rhs[13] + lhs[8] * rhs[14] + lhs[12] * rhs[15];
	result[13] = lhs[1] * rhs[12] + lhs[5] * rhs[13] + lhs[9] * rhs[14] + lhs[13] * rhs[15];
	result[14] = lhs[2] * rhs[12] + lhs[6] * rhs[13] + lhs[10] * rhs[14] + lhs[14] * rhs[15];
	result[15] = lhs[3] * rhs[12] + lhs[7] * rhs[13] + lhs[11] * rhs[14] + lhs[15] * rhs[15];
}

void MyMatrix::Normalise3(float* vector) {

	float magnitude = sqrt((vector[0] * vector[0]) + (vector[1] * vector[1]) + (vector[2] * vector[2]));

	vector[0] = vector[0] / magnitude;
	vector[1] = vector[1] / magnitude;
	vector[2] = vector[2] / magnitude;
}

void MyMatrix::MultiplyVecByMatrix(float* vec3, const float* mat4) {

	float newx = mat4[0] * vec3[0] + mat4[4] * vec3[1] + mat4[8] * vec3[2] + mat4[12];
	float newy = mat4[1] * vec3[0] + mat4[5] * vec3[1] + mat4[9] * vec3[2] + mat4[13];
	float newz = mat4[2] * vec3[0] + mat4[6] * vec3[1] + mat4[10] * vec3[2] + mat4[14];

	vec3[0] = newx;
	vec3[1] = newy;
	vec3[2] = newz;
}

void MyMatrix::RotateMat4ByVec3(float* mat4, const float* vec3, const float rotation) {

	MyMatrix::RotateX(mat4, (vec3[0] * rotation));
	MyMatrix::RotateY(mat4, (vec3[1] * rotation));
	MyMatrix::RotateZ(mat4, (vec3[2] * rotation));
}

// expensive but called infrequently
float MyMatrix::length(float* vec3) {

	return (sqrt((pow(vec3[0], 2)) + (pow(vec3[1], 2)) + (pow(vec3[2], 2))));
}

float MyMatrix::distance(float* veca, float* vecb) {

	float difference[3] = {
		vecb[0] - veca[0],
		vecb[1] - veca[1],
		vecb[2] - veca[2]
	};

	return MyMatrix::length(difference);
}

void MyMatrix::Scale(float* vec3, const float scale) {

	vec3[0] *= scale;
	vec3[1] *= scale;
	vec3[2] *= scale;
}

void MyMatrix::RotateXYZ(float* matrix, const float degsX, const float degsY, const float degsZ) {

	MyMatrix::RotateX(matrix, degsX);
	MyMatrix::RotateY(matrix, degsY);
	MyMatrix::RotateZ(matrix, degsZ);
}

// as described by the Mesa 3D Graphics Library
// https://www.mesa3d.org/
void MyMatrix::InvertMat4(const float* m, float* invOut)
{
	float inv[16], det;
	int i;

	// ===========================
	// X column
	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];


	// ===========================
	// Y column
	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];


	// ===========================
	// Z column
	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];


	// ===========================
	// homogenious column
	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det != 0) {

		det = 1.0 / det;

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;
	}
	else {

		for (i = 0; i < 16; i++)
			invOut[i] = m[i];
	}
}

void MyMatrix::TransposeMat4(const float* m, float* transpose) {

	transpose[0] = m[0];
	transpose[1] = m[4];
	transpose[2] = m[8];
	transpose[3] = m[12];

	transpose[4] = m[1];
	transpose[5] = m[5];
	transpose[6] = m[9];
	transpose[7] = m[13];

	transpose[8] = m[2];
	transpose[9] = m[6];
	transpose[10] = m[10];
	transpose[11] = m[14];

	transpose[12] = m[3];
	transpose[13] = m[7];
	transpose[14] = m[11];
	transpose[15] = m[15];
}

void MyMatrix::CreateNormalMatrix(const float* model, float* normal) {

	float invModel[16];
	MyMatrix::InvertMat4(model, invModel);
	MyMatrix::TransposeMat4(invModel, normal);
}

void MyMatrix::SetOrtho(float* matrix, const float left, const float right, const float bottom, const float top, const float znear, const float zfar) {

	matrix[0] = (2.0f / right - left);
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = (2.0f / top - bottom);
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = 0.0f;
	matrix[9] = 0.0f;
	matrix[10] = -(2.0f / zfar - znear);
	matrix[11] = 0.0f;

	matrix[12] = -((right + left) / (right - left));
	matrix[13] = -((top + bottom) / (top - bottom));
	matrix[14] = -((zfar + znear) / (zfar - znear));
	matrix[15] = 1.0f;
}

void MyMatrix::SetFrustum(float* matrix, const float left, const float right, const float bottom, const float top, const float znear, const float zfar) {

	matrix[0] = (2.0f * znear) / (right - left);
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;

	matrix[4] = 0.0f;
	matrix[5] = (2.0f * znear) / (top - bottom);
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;

	matrix[8] = (right + left) / (right - left);
	matrix[9] = (top + bottom) / (top - bottom);
	matrix[10] = -((zfar + znear) / (zfar - znear));
	matrix[11] = -1.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = -((2.0f * zfar * znear) / (zfar - znear));
	matrix[15] = 0.0f;
}
//
//void MyMatrix::SetLookAt(float* matrix, const float* eye, const float* centre, const float* up) {
//
//	// forewards = centre - eye
//	float forwards[3] = {
//
//		centre[0] - eye[0],
//		centre[1] - eye[1],
//		centre[2] - eye[2]
//	};
//	MyMatrix::Normalise3(forwards);
//
//	// normalise the up direction
//	float normUp[3] = { up[0], up[1], up[2] };
//	MyMatrix::Normalise3(normUp);
//
//	// side direction = forward X normUp
//	float side[3]; // = { forwards[0], forwards[1], forwards[2] };
//	MyMatrix::CrossProduct3(forwards, normUp, side);
//	MyMatrix::Normalise3(side);
//
//	// cross product up = side X forwards
//	float crossUp[3]; // = { side[0], side[1], side[2] };
//	MyMatrix::CrossProduct3(side, forwards, crossUp);
//	MyMatrix::Normalise3(crossUp);
//
//	// set up lookat matrix with given values
//	matrix[0] = side[0];
//	matrix[1] = crossUp[0];
//	matrix[2] = -forwards[0];
//	matrix[3] = 0;
//
//	matrix[4] = side[1];
//	matrix[5] = crossUp[1];
//	matrix[6] = -forwards[1];
//	matrix[7] = 0;
//
//	matrix[8] = side[2];
//	matrix[9] = crossUp[2];
//	matrix[10] = -forwards[2];
//	matrix[11] = 0;
//
//	matrix[12] = -MyMatrix::DotProduct3(side, eye);
//	matrix[13] = -MyMatrix::DotProduct3(crossUp, eye);
//	matrix[14] = MyMatrix::DotProduct3(forwards, eye);
//	matrix[15] = 1;
//}
//
//void MyMatrix::SetLookAtGLM(float* matrix, const glm::vec3 eye, const glm::vec3 centre, const glm::vec3 up) {
//
//	// forewards = centre - eye
//	glm::vec3 forwards = glm::normalize(centre - eye);
//
//	// normalise the up direction
//	glm::vec3 normUp = glm::normalize(up);
//
//	// side direction = forward X normUp
//	glm::vec3 side = glm::normalize(glm::cross(forwards, normUp));
//
//	// cross product up = side X forwards
//	glm::vec3 crossUp = glm::normalize(glm::cross(side, forwards));
//
//	// set up lookat matrix with given values
//	matrix[0] = side.x;
//	matrix[1] = crossUp.x;
//	matrix[2] = -forwards.x;
//	matrix[3] = 0;
//
//	matrix[4] = side.y;
//	matrix[5] = crossUp.y;
//	matrix[6] = -forwards.y;
//	matrix[7] = 0;
//
//	matrix[8] = side.z;
//	matrix[9] = crossUp.z;
//	matrix[10] = -forwards.z;
//	matrix[11] = 0;
//
//	matrix[12] = -glm::dot(side, eye);
//	matrix[13] = -glm::dot(crossUp, eye);
//	matrix[14] = glm::dot(forwards, eye);
//	matrix[15] = 1;
//}