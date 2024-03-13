#ifndef Enemy_hpp_INCLUDED
#define Enemy_hpp_INCLUDED

#include "Entity.hpp"
#include "Sprite.hpp"

#include <raylib.h>

#include <chrono>
#include <optional>

class Enemy : public Entity
{
	public:

	Enemy() : 
		sprite_(3, 4, 3, "assets/Enemy_sprite_sheet/enemy_one.png")
	{

	}

	Vector2 update(std::chrono::nanoseconds del_time, Game_view game, const Vector2& position) override;
	void render(Vector2 position) override;
	Vector2 render_size() const override;

	private:

	Sprite sprite_;
	int current_animation_{0};
	int pixels_per_second_{150};
	bool is_moving_{false};
	
	std::optional<Vector2> select_target_tile(Vector2 enemy_position, Vector2 player_position, const Game_view game);
};

#endif // Enemy_hpp_INCLUDED

