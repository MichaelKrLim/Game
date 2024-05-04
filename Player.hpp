#ifndef Player_hpp_INCLUDED
#define Player_hpp_INCLUDED

#include "Entity.hpp"
#include "Resource_buffer.hpp"
#include "Sprite.hpp"

#include <raylib.h>

#include <chrono>

class Player : public Entity
{
	public:

	explicit Player(const Resource_buffer& resource_buffer) : 
		sprite_(8, 2, 4, "assets/player.png", resource_buffer)
	{

	}

	Vector2 update(std::chrono::nanoseconds del_time, Game_view game, const Vector2&) override;
	void render(Vector2 position) override;
	Vector2 render_size() const override;

	private:
	enum class Direction
	{
		left, right
	};
	Sprite sprite_;
	//
	bool ISHITTINGANYTHING{false};
	//
	std::chrono::nanoseconds time_elapsed_since_last_attack_{0};
	Direction facing{Direction::right};
	int current_animation_{0};
	int pixels_per_second_{1000};
	bool is_moving_{false};
	bool is_attacking_{false};
	bool is_armed_{true};

	Vector2 handle_movement(std::chrono::nanoseconds);
};

#endif // Player_hpp_INCLUDED

