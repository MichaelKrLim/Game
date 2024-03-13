#ifndef Entity_hpp_INCLUDED
#define Entity_hpp_INCLUDED

#include <chrono>

#include <raylib.h>

class Game_view;

class Entity
{
	public:

	virtual Vector2 update(std::chrono::nanoseconds del_time, Game_view game, const Vector2& position) = 0;
	virtual void render(Vector2 position) = 0;
	virtual Vector2 render_size() const = 0;
};

#endif // Entity_hpp_INCLUDED

