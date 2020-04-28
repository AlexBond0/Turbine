#pragma once

#include "Matrix.h"
#include <windows.h>

class MoveablePoint
{
public:

	MoveablePoint();
	MoveablePoint(MoveablePoint* copy);

	~MoveablePoint() {};

	void SetWorldPos(byte* buffer);
	void SetWorldPos(glm::vec3 pos);
	glm::vec3* GetWorldPosVec();

	void SetLocalPos(byte* buffer);
	void SetLocalPos(float x, float y, float z);
	void SetLocalPos(glm::vec3 pos);
	glm::vec3* GetLocalPosVec();

	void SetTranslation(byte* buffer);
	void SetTranslation(float x, float y, float z);
	void SetTranslation(glm::vec3 pos);
	glm::vec3* GetTranlationVec();

	void SetUp(glm::vec3 direction);
	glm::vec3* GetUpVec();

	void SetScale(byte* buffer);
	void SetScale(float x, float y, float z);
	void SetScale(float newScale);
	void SetScale(glm::vec3 newScale);
	glm::vec3* GetScaleVec();

protected:

	glm::vec3 _worldPos;		// world position of moveable element | Depreciated
	glm::vec3 _localPos;		// local position of moveable element
	glm::vec3 _translation;		// local translation of moveable element
	glm::vec3 _scale;			// scale of element
	glm::vec3 _up;				// local up dimention for moveable element
};

