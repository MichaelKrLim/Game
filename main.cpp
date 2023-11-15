#include "Map.hpp"
#include "Tile_set.hpp"

#include "rl/Texture.hpp"

#include <raylib.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

class Player
{

	public:

	void render() const;
		
	private:

	rl::Texture texture_{"assets/player.png"};

	const int frames_per_row = 8;
	const int num_rows = 2;
	const int frames_per_animation = 4;
};

int main()
{
	InitWindow(1280, 720, "Game");
	Player player;
	RenderTexture image = LoadRenderTexture(1280*2, 720*2);
	SetExitKey(KEY_SPACE); 
	Tile_set terrain("assets/tileset.png", 32);
	Map base_map({"assets/first_terrain.csv", "assets/second_trees.csv"});
	while(!WindowShouldClose())
	{
		BeginTextureMode(image);
		// rendering 
		ClearBackground(WHITE);
		base_map.render(terrain, {0, 0});
		//
		EndTextureMode();
		BeginDrawing();
		DrawTexturePro(image.texture, Rectangle(0, 0, image.texture.width,-image.texture.height), Rectangle(0, 0, image.texture.width*0.5, image.texture.height*0.5), Vector2(0,0), 0, WHITE);
		EndDrawing();
	}
	UnloadRenderTexture(image);
}
