#ifndef Game_hpp_INCLUDED
#define Game_hpp_INCLUDED

#include "Entity.hpp"
#include "Map.hpp"
#include "rl/operator_overloads.hpp"
#include "Tile_set.hpp"

#include <raylib.h>

#include <memory>
#include <unordered_map>
#include <unordered_set>

class Game_view;

class Game
{

	public:
	
	Game();
	~Game();
	void update(std::chrono::nanoseconds del_time);
	bool sprite_can_move_to_without_collision(const Vector2& destination, const Vector2& sprite_size, const Vector2& starting_position);
	void display();
	
	private:

	Game& operator = (const Game&) = delete;
	Game(const Game&) = delete;

	friend class Game_view;

	Map current_map_ = Map({"assets/texture_layers/Tile_map_Grass.csv", "assets/texture_layers/Tile_map_L2.csv"}, "assets/Tile_map_Collison.csv");
	Tile_set current_tile_set_ = Tile_set("assets/tileset.png", 32);
	struct Positioned_entity
	{
		std::unique_ptr<Entity> entity;
		Vector2 position;
	};

	RenderTexture render_texture_ = LoadRenderTexture(GetRenderWidth()*2, GetRenderHeight()*2);
	double camera_fraction_of_resolution_bound_ = 3.0/4.0;
	double camera_velocity_dampening_multiplier_ = 0.7;
	double camera_velocity_multiplier_ = 0.5;	
	double zoom_{1};
	const int player_index_ = 0;
	std::vector<Positioned_entity> entities_{};
	Vector2 camera_position_{0, 0};
	Vector2 camera_velocity_{0, 0};
	const int cell_size_ = 64;
	std::unordered_map<Vector2, std::vector<std::size_t>> entity_buckets_;

	void insert_into_entity_bucket(std::size_t entity_id);
	std::unordered_set<Vector2> get_entity_cell_ids(std::size_t entity_id) const;
	void add_point_to_cell_ids(Vector2 position, std::unordered_set<Vector2> &cell_ids) const;
	bool position_is_free(const Vector2& position, const Vector2& sprite_size) const;
	Vector2 player_position() const
	{
		return entities_[player_index_].position;
	}

	void update_camera(std::chrono::nanoseconds);
	void render();
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
	std::vector<std::size_t> collisions_with(Rectangle collision_box)
	{
		std::vector<std::size_t> colliding_entites_ids;
		for(std::size_t i{0}; i<game_.entities_.size(); ++i)
		{
			const auto& current_entity = game_.entities_[i].entity;
			Rectangle entity_collision_box
			{
				.x=game_.entities_[i].position.x,
				.y=game_.entities_[i].position.y,
				.width=current_entity->render_size().x,
				.height=current_entity->render_size().y
			};
			if(CheckCollisionRecs(collision_box, entity_collision_box))
				colliding_entites_ids.push_back(i);
		}
		return colliding_entites_ids;
	}

	private:

		Game& game_;
};

#endif // Game_hpp_INCLUDED

