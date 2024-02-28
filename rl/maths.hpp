#ifndef rl_maths_hpp_INCLUDED
#define rl_maths_hpp_INCLUDED

#include "operator_overloads.hpp"

#include "raylib.h"

#include <cassert>
#include <cmath>

inline float magnitude(Vector2 vector)
{
	return std::hypot(vector.x, vector.y);
}

inline Vector2 normalise(Vector2 vector)
{
	assert(magnitude(vector) != 0);
	return vector/magnitude(vector);
}

inline float distance(Vector2 first, Vector2 second)
{
	return magnitude(first-second);
}

#endif // rl/maths_hpp_INCLUDED
