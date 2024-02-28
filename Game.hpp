#ifndef Game_hpp_INCLUDED
#define Game_hpp_INCLUDED

#include "Entity.hpp"
#include "Map.hpp"
#include "rl/operator_overloads.hpp"
#include "Tile_set.hpp"

#include <raylib.h>

#include <memory>

class Game
{
	private:

	Map current_map_ = Map({"assets/texture_layers/first_terrain.csv", "assets/texture_layers/second_trees.csv"}, "assets/collision map.csv");
	Tile_set current_tile_set_ = Tile_set("assets/tileset.png", 32);
	struct Positioned_entity
	{
		std::unique_ptr<Entity> entity;
		Vector2 position;
	};
	const int player_index_ = 0;
	std::vector<Positioned_entity> entities_{};

	public:
	
	Game();
	void update(std::chrono::nanoseconds del_time);
	void render();
	bool position_is_free(const Vector2& position, const Vector2& sprite_size) const;
	bool sprite_can_move_to_without_collision(const Vector2& destination, const Vector2& sprite_size, const Vector2& starting_position);
	
	Vector2 map_size() const
	{
		return current_map_.size()*current_tile_set_.tile_size();
	}
	Vector2 player_position() const
	{
		return entities_[player_index_].position;
	}
};

#endif // Game_hpp_INCLUDED

