#include "Enemy.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include "Sprite.hpp"
#include "Tile_set.hpp"

#include "rl/maths.hpp"
#include "rl/operator_overloads.hpp"
#include "rl/Texture.hpp"

#include <raylib.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

bool position_is_free(const Map& map, const Tile_set& tile_set, const Vector2& position, const Vector2& sprite_size)
{
	return (map.is_reachable(position, tile_set) &&
			map.is_reachable(position+Vector2{sprite_size.x,0}, tile_set) &&
			map.is_reachable(position+sprite_size, tile_set) &&
			map.is_reachable(position+Vector2{0,sprite_size.y}, tile_set));
}

bool sprite_can_move_to(const Map& map, const Tile_set& tile_set, const Vector2& destination, const Vector2& sprite_size, const Vector2& starting_position)
{
	const auto step_size = std::min(sprite_size.x, sprite_size.y);
	for(int i{}; i<static_cast<int>(magnitude(destination-starting_position)/step_size); ++i)
	{
		if(!position_is_free(map, tile_set, starting_position+i*(normalise(destination-starting_position)*step_size), sprite_size))
			return false;
	}

	if(!position_is_free(map,tile_set, destination, sprite_size))
		return false;

	return true;
}

int main()
{
	using namespace std::chrono_literals;
	
	InitWindow(1280, 720, "Game");
	SetTargetFPS(240);

	Enemy enemy;
	Vector2 enemy_position = {100, 100};
	Tile_set terrain("assets/tileset.png", 32);
	Map map({"assets/texture_layers/first_terrain.csv", "assets/texture_layers/second_trees.csv"}, "assets/collision map.csv");
	Player player;
	Vector2 player_position{80, 80};
	RenderTexture image = LoadRenderTexture(GetRenderWidth()*2, GetRenderHeight()*2);
	SetExitKey(KEY_SPACE); 
	auto del_time_elapsed = 0ns;
	while(!WindowShouldClose())
	{
		auto begin_time = std::chrono::steady_clock::now();
				
		BeginTextureMode(image);
		// rendering 
		ClearBackground(WHITE);
		map.render(terrain, {0, 0});
		enemy.render(enemy_position);
		player.render(player_position);
		//
		EndTextureMode();
		BeginDrawing();
		DrawTexturePro(image.texture, Rectangle(0, 0, image.texture.width,-image.texture.height), Rectangle(0, 0, image.texture.width*0.5, image.texture.height*0.5), Vector2(0,0), 0, WHITE);
		EndDrawing();

		auto end_time = std::chrono::steady_clock::now();	
		auto player_del_position = player.update(end_time-begin_time);
		Vector2 new_unvalidated_position = player_position+player_del_position;
		if(sprite_can_move_to(map, terrain, new_unvalidated_position, player.render_size(), player_position))
		{
			player_position = new_unvalidated_position;
		}

		auto enemy_del_position = enemy.update(end_time-begin_time, enemy_position, map.size()*terrain.tile_size());
		Vector2 ne_unvalidated_position = enemy_position+enemy_del_position;
		if(sprite_can_move_to(map, terrain, ne_unvalidated_position, player.render_size(), enemy_position))
		{
			enemy_position = ne_unvalidated_position;
		}
	}
	UnloadRenderTexture(image);
}
