#include "Map.hpp"
#include "Tile_set.hpp"

#include "rl/Texture.hpp"

#include <raylib.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

class Player
{
	public:

	void render(int animation_number) const;
	void update(std::chrono::nanoseconds del_time);
	
	private:

	rl::Texture texture_ {"assets/player.png"};

	const int frames_per_row_ = 8;
	const int num_rows_ = 2;
	const int frames_per_animation_ = 4;
	const int frame_width_ = texture_.width()/frames_per_row_;
	const int frame_height_ = texture_.height()/num_rows_;

	int frame_number_{0};
	std::chrono::nanoseconds del_time_elapsed_{0}; 
};

void Player::update(std::chrono::nanoseconds del_time)
{
	del_time_elapsed_ += del_time;
	while(del_time_elapsed_ >= std::chrono::seconds{1})
	{
		++frame_number_;
		del_time_elapsed_ -= std::chrono::seconds{1};
	}
	frame_number_%=frames_per_animation_;
}

void Player::render(int animation_number) const
{
	const int image_number = frame_number_+animation_number*frames_per_animation_;
	const Rectangle frame(image_number*frame_width_, (image_number/frames_per_row_)*frame_height_, frame_width_, frame_height_);
	DrawTexturePro(texture_, frame, Rectangle(50, 50, frame_width_*3, frame_height_*3), {0, 0}, 0, WHITE);
	auto end_time = std::chrono::steady_clock::now();
}

int main()
{
	using namespace std::chrono_literals;
	
	InitWindow(1280, 720, "Game");
	SetTargetFPS(240);
	Player player;
	RenderTexture image = LoadRenderTexture(1280*2, 720*2);
	SetExitKey(KEY_SPACE); 
	Tile_set terrain("assets/tileset.png", 32);
	Map base_map({"assets/first_terrain.csv", "assets/second_trees.csv"});
	auto del_time_elapsed = 0ns;
	while(!WindowShouldClose())
	{
		auto begin_time = std::chrono::steady_clock::now();
		auto space_pressed = IsKeyDown(KEY_ESCAPE);
		
		BeginTextureMode(image);
		// rendering 
		ClearBackground(WHITE);
		base_map.render(terrain, {0, 0});
		player.render(0);
		//
		EndTextureMode();
		BeginDrawing();
		DrawTexturePro(image.texture, Rectangle(0, 0, image.texture.width,-image.texture.height), Rectangle(0, 0, image.texture.width*0.5, image.texture.height*0.5), Vector2(0,0), 0, WHITE);
		EndDrawing();

		auto end_time = std::chrono::steady_clock::now();
		if(!space_pressed)
		{
			player.update(end_time-begin_time);
		}
	}
	UnloadRenderTexture(image);
}
