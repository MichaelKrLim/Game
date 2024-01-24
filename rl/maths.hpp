#ifndef rl_maths_hpp_INCLUDED
#define rl_maths_hpp_INCLUDED

#include "operator_overloads.hpp"

#include "raylib.h"

#include <cmath>

inline float magnitude(Vector2 vector)
{
	return std::hypot(vector.x, vector.y);
}

inline Vector2 normalise(Vector2 vector)
{
	return vector/magnitude(vector);
}

#endif // rl/maths_hpp_INCLUDED

