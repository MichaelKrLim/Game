#include "Enemy.hpp"
#include "Entity.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Sprite.hpp"
#include "Tile_set.hpp"

#include "rl/maths.hpp"
#include "rl/operator_overloads.hpp"
#include "rl/Texture.hpp"

#include <raylib.h>

#include <chrono>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

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
	bool position_is_free(const Map& map, const Tile_set& tile_set, const Vector2& position, const Vector2& sprite_size)
	{
		return (map.is_reachable(position, tile_set) &&
				map.is_reachable(position+Vector2{sprite_size.x,0}, tile_set) &&
				map.is_reachable(position+sprite_size, tile_set) &&
				map.is_reachable(position+Vector2{0,sprite_size.y}, tile_set));
	}	

	public:
	
	Game()
	{
		entities_.emplace_back(std::make_unique<Player>(), Vector2{80, 80});
		entities_.emplace_back(std::make_unique<Enemy>(), Vector2{100, 100});
	}

	void update(std::chrono::nanoseconds del_time)
	{
		for(auto& positioned_entity : entities_)
		{	
			auto del_position = positioned_entity.entity->update(del_time, *this);
			Vector2 new_unvalidated_position = positioned_entity.position+del_position;
			if(sprite_can_move_to(current_map_, current_tile_set_, new_unvalidated_position, positioned_entity.entity->render_size(), positioned_entity.position))
			{
				positioned_entity.position = new_unvalidated_position;
			}
		}
	}

	void render()
	{
		ClearBackground(WHITE);
		current_map_.render(current_tile_set_, {0, 0});
		for(const auto& positioned_entity : entities_)
		{
			positioned_entity.entity->render(positioned_entity.position);
		}
	}
	
	bool sprite_can_move_to(const Map& map, const Tile_set& tile_set, const Vector2& destination, const Vector2& sprite_size, const Vector2& starting_position)
	{
		const auto step_size = std::min(sprite_size.x, sprite_size.y);
		for(int i{}; i<static_cast<int>(magnitude(destination-starting_position)/step_size); ++i)
		{
			if(!position_is_free(map, tile_set, starting_position+i*(normalise(destination-starting_position)*step_size), sprite_size))
				return false;
		}

		if(!position_is_free(map, tile_set, destination, sprite_size))
			return false;

		return true;
	}
};

int main()
{
	using namespace std::chrono_literals;
	
	InitWindow(1280, 720, "Game");
	SetTargetFPS(240);

	Game game;
	RenderTexture image = LoadRenderTexture(GetRenderWidth()*2, GetRenderHeight()*2);
	SetExitKey(KEY_SPACE); 
	auto del_time_elapsed = 0ns;
	while(!WindowShouldClose())
	{
		auto begin_time = std::chrono::steady_clock::now();
				
		BeginTextureMode(image);
		
			game.render();
		
		EndTextureMode();
		BeginDrawing();
		
			DrawTexturePro(image.texture, Rectangle(0, 0, image.texture.width,-image.texture.height), Rectangle(0, 0, image.texture.width*0.5, image.texture.height*0.5), Vector2(0,0), 0, WHITE);

		EndDrawing();

		auto end_time = std::chrono::steady_clock::now();
		game.update(end_time-begin_time);
	}
	UnloadRenderTexture(image);
}
