#pragma once

#pragma once
#define _USE_MATH_DEFINES
#include <math.h> // remember to add _USE_MATH_DEFINES to the project settings
#include <memory.h>

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm\gtx\vector_angle.hpp"
#include "glm\gtx\intersect.hpp"
#include "glm\gtx\euler_angles.hpp"
#include "glm\gtx\quaternion.hpp"

#define DEGSTORADS(x)     ((x*M_PI)/180.0f)
#define RADSTODEGS(x)     ((x/M_PI)*180.0f)

