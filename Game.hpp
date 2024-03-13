#ifndef Game_hpp_INCLUDED
#define Game_hpp_INCLUDED

#include "Entity.hpp"
#include "Map.hpp"
#include "rl/operator_overloads.hpp"
#include "Tile_set.hpp"

#include <raylib.h>

#include <memory>

class Game_view;

class Game
{
	
	public:
	
	Game();
	void update(std::chrono::nanoseconds del_time);
	void render();
	bool sprite_can_move_to_without_collision(const Vector2& destination, const Vector2& sprite_size, const Vector2& starting_position);
	
	private:

	friend class Game_view;
	
	Map current_map_ = Map({"assets/texture_layers/first_terrain.csv", "assets/texture_layers/second_trees.csv"}, "assets/collision map.csv");
	Tile_set current_tile_set_ = Tile_set("assets/tileset.png", 32);
	struct Positioned_entity
	{
		std::unique_ptr<Entity> entity;
		Vector2 position;
	};
	const int player_index_ = 0;
	std::vector<Positioned_entity> entities_{};
	Vector2 camera_position{0, 0};
	Vector2 camera_velocity{0, 0};
	
	bool position_is_free(const Vector2& position, const Vector2& sprite_size) const;
	Vector2 player_position() const
	{
		return entities_[player_index_].position;
	}
};

class Game_view
{

	public:
		
	Game_view(Game& game) : game_(game) {}
	Vector2 player_position() const
	{
		return game_.player_position();
	}
	Vector2 map_size_in_tiles() const
	{
		return game_.current_map_.size();
	}
	int tile_size_in_pixels() const
	{
		return game_.current_tile_set_.tile_size();
	}
	bool position_is_free(const Vector2& position, const Vector2& sprite_size) const
	{
		return game_.position_is_free(position, sprite_size);
	}

	private:

		Game& game_;
};

#endif // Game_hpp_INCLUDED

