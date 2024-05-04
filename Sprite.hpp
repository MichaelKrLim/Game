#ifndef Sprite_hpp_INCLUDED
#define Sprite_hpp_INCLUDED

#include "Resource_buffer.hpp"

#include "rl/Texture.hpp"

#include <raylib.h>

#include <chrono>
#include <filesystem>
#include <iostream>
#include <memory>


class Sprite
{
	public:

	Sprite(const int frames_per_row, const int num_rows, const int frames_per_animation, std::filesystem::path texture_path, const Resource_buffer& resource_buffer) :
		frames_per_row_(frames_per_row), 
		num_rows_(num_rows), 
		frames_per_animation_(frames_per_animation),
		texture_(resource_buffer.get_texture(texture_path))
	{

	}

	void render(int animation_number, Vector2 position) const;
	void update(std::chrono::nanoseconds del_time);
	void reset_animation();
	Vector2 frame_size() const;

	private:

	std::shared_ptr<rl::Texture> texture_;
	std::chrono::nanoseconds nanoseconds_per_frame_{std::chrono::milliseconds{100}};
	const int frames_per_row_;
	const int num_rows_;
	const int frames_per_animation_;
	const int frame_width_ = texture_->width()/frames_per_row_;
	const int frame_height_ = texture_->height()/num_rows_;

	int frame_number_{0};
	std::chrono::nanoseconds del_time_elapsed_{0}; 
};
#endif // Sprite_hpp_INCLUDED

