#include "Enemy.hpp"
#include "Entity.hpp"
#include "Game.hpp"
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

			const auto source_rectangle = Rectangle(0, 0, image.texture.width,-image.texture.height);
			const auto destination_rectangle = Rectangle(0, 0, image.texture.width*0.5, image.texture.height*0.5);
			DrawTexturePro(image.texture, source_rectangle, destination_rectangle, Vector2(0,0), 0, WHITE);

		EndDrawing();

		auto end_time = std::chrono::steady_clock::now();
		game.update(end_time-begin_time);
	}
	UnloadRenderTexture(image);
}
