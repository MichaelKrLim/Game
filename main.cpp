#include "Layer.hpp"
#include "Tile_set.hpp"

#include <raylib.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

int main()
{
	InitWindow(1280, 720, "Game");
	RenderTexture image = LoadRenderTexture(1280*2, 720*2);
	SetExitKey(KEY_SPACE); 
	Tile_set terrain("assets/tileset.png", 32);
	Layer base_layer("assets/first_terrain.csv"); 
	Layer trees("assets/second_trees.csv");
	while(!WindowShouldClose())
	{
		BeginTextureMode(image);
		// rendering 
		ClearBackground(WHITE);
		base_layer.render(terrain, Vector2(0, 0));
		trees.render(terrain, Vector2(0, 0));
		//
		EndTextureMode();
		BeginDrawing();
		DrawTexturePro(image.texture, Rectangle(0, 0, image.texture.width,-image.texture.height), Rectangle(0, 0, image.texture.width*0.5, image.texture.height*0.5), Vector2(0,0), 0, WHITE);
		EndDrawing();
	}
	UnloadRenderTexture(image);
}
