#ifndef CHEER_MATH_H
#define CHEER_MATH_H

#include <math.h>
//GLMath
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace chM
{
	static void setBezSpline(float& value, float elapsedTime, float completionTime = 1.0f,
		float x0 = 0.0f, float wp = 0.0f, float dest = 0.0f)
	{
		float t = elapsedTime / completionTime;
		value = (t - 1) * (t - 1) + 2.0 * (t - 1) * t * wp + t * t * dest;
	}
}

#endif