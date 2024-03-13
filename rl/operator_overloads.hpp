#ifndef rl_operator_overloads_hpp_INCLUDED
#define rl_operator_overloads_hpp_INCLUDED

#include <raylib.h>

#include <concepts>
#include <iostream>

inline std::ostream& operator<<(std::ostream& out, const Vector2& output)
{
	return out << '{' << output.x << ", " << output.y << '}';
}

template <typename T>
Vector2 operator*(Vector2 lhs, T rhs) requires std::integral<T> || std::floating_point<T>
{
	return Vector2(lhs.x*rhs, lhs.y*rhs);
}

template <typename T>
Vector2 operator*(T lhs, Vector2 rhs) requires std::integral<T> || std::floating_point<T>
{
	return rhs*lhs;
}

inline Vector2 operator-(Vector2 vector)
{
	return Vector2(-vector.x, -vector.y);
}

inline Vector2 operator-(Vector2 lhs, Vector2 rhs)
{
	return Vector2(lhs.x-rhs.x,lhs.y-rhs.y);
}

inline Vector2 operator+(Vector2 lhs, Vector2 rhs)
{
	return Vector2(lhs.x+rhs.x,lhs.y+rhs.y);
}

inline Vector2& operator+=(Vector2& lhs, Vector2 rhs)
{
	return lhs=lhs+rhs;
}

template <typename T>
Vector2 operator/(Vector2 lhs, T rhs) requires std::integral<T> || std::floating_point<T>
{
	return Vector2(lhs.x/rhs, lhs.y/rhs);
}

inline bool operator==(Vector2 lhs, Vector2 rhs)
{
	return lhs.x==rhs.x && lhs.y==rhs.y;
}

#endif // rl/operator_overloads_hpp_INCLUDED
