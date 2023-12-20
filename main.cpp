#include "Map.hpp"
#include "Player.hpp"
#include "Sprite.hpp"
#include "Tile_set.hpp"

#include "rl/Texture.hpp"

#include <raylib.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>


int main()
{
	using namespace std::chrono_literals;
	
	InitWindow(1280, 720, "Game");
	SetTargetFPS(240);
	
	Tile_set terrain("assets/tileset.png", 32);
	Map base_map({"assets/texture_layers/first_terrain.csv", "assets/texture_layers/second_trees.csv"}, "assets/collision map.csv");
	Player player;
	Vector2 player_position{80, 80};
	RenderTexture image = LoadRenderTexture(GetRenderWidth()*2, GetRenderHeight()*2);
	SetExitKey(KEY_SPACE); 
	auto del_time_elapsed = 0ns;
	while(!WindowShouldClose())
	{
		auto begin_time = std::chrono::steady_clock::now();
		auto space_pressed = IsKeyDown(KEY_ESCAPE);
		
		BeginTextureMode(image);
		// rendering 
		ClearBackground(WHITE);
		base_map.render(terrain, {0, 0});
		player.render(player_position);
		//
		EndTextureMode();
		BeginDrawing();
		DrawTexturePro(image.texture, Rectangle(0, 0, image.texture.width,-image.texture.height), Rectangle(0, 0, image.texture.width*0.5, image.texture.height*0.5), Vector2(0,0), 0, WHITE);
		EndDrawing();

		auto end_time = std::chrono::steady_clock::now();
		if(!space_pressed)
		{
			auto del_position = player.update(end_time-begin_time);
			Vector2 new_unvalidated_position = {player_position.x+del_position.x, player_position.y + del_position.y};
			if(base_map.is_reachable(new_unvalidated_position, terrain))
			{
				player_position = new_unvalidated_position;
			}
		}
	}
	UnloadRenderTexture(image);
}
