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
	
	InitWindow(0, 0, "Game");
	SetTargetFPS(240);

	Game game;
	SetWindowMonitor(0);

	auto del_time_elapsed = 0ns;
	while(!WindowShouldClose())
	{
		if(IsKeyPressed(KEY_F11))
			ToggleFullscreen();

		auto begin_time = std::chrono::steady_clock::now();
		game.display();
		auto end_time = std::chrono::steady_clock::now();
		game.update(end_time-begin_time);
	}
}
