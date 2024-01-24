#ifndef Player_hpp_INCLUDED
#define Player_hpp_INCLUDED

#include "Sprite.hpp"

#include <raylib.h>

#include <chrono>

class Player
{
	public:

	Player() : 
		sprite_(8, 2, 4, "assets/player.png")
	{

	}

	Vector2 update(std::chrono::nanoseconds del_time);
	void render(Vector2 position);
	Vector2 render_size() const;

	private:

	Sprite sprite_;
	int current_animation_{0};
	int pixels_per_second_{1000};
	bool is_moving_{false};

	Vector2 handle_movement(std::chrono::nanoseconds);
};

#endif // Player_hpp_INCLUDED

