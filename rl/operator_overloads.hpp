#ifndef rl_operator_overloads_hpp_INCLUDED
#define rl_operator_overloads_hpp_INCLUDED

#include <raylib.h>

#include <concepts>

template <typename T>
inline Vector2 operator*(Vector2 lhs, T rhs) requires std::integral<T> || std::floating_point<T>
{
	return Vector2(lhs.x*rhs, lhs.y*rhs);
}

inline Vector2 operator-(Vector2 lhs, Vector2 rhs)
{
	return Vector2(lhs.x-rhs.x,lhs.y-rhs.y);
}

inline Vector2 operator+(Vector2 lhs, Vector2 rhs)
{
	return Vector2(lhs.x+rhs.x,lhs.y+rhs.y);
}

template <typename T>
inline Vector2 operator/(Vector2 lhs, T rhs) requires std::integral<T> || std::floating_point<T>
{
	return Vector2(lhs.x/rhs, lhs.y/rhs);
}

#endif // rl/operator_overloads_hpp_INCLUDED
