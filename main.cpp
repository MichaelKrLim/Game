#include "Tile_set.hpp"

#include <raylib.h>
#include <filesystem>

int main()
{
	InitWindow(1920, 1080, "Game");
	SetExitKey(KEY_SPACE); 
	Tile_set terrain("assets/tileset.png", 32);
	Tile_set trees("assets/tileset.png", 32);
	while(!WindowShouldClose())
	{
		BeginDrawing();
		// rendering 
		ClearBackground(WHITE);
		terrain.draw(1586, {0,0});
		//
		EndDrawing();
	}
}
