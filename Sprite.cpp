#include "Sprite.hpp"

void Sprite::update(std::chrono::nanoseconds del_time)
{
	del_time_elapsed_ += del_time;
	while(del_time_elapsed_ >= nanoseconds_per_frame_)
	{
		++frame_number_;
		del_time_elapsed_ -= nanoseconds_per_frame_;
	}
	frame_number_%=frames_per_animation_;
}

void Sprite::render(int animation_number, Vector2 position) const
{
	const int image_number = frame_number_+animation_number*frames_per_animation_;
	const Rectangle frame(image_number*frame_width_, (image_number/frames_per_row_)*frame_height_, frame_width_, frame_height_);
	DrawTexturePro(texture_, frame, Rectangle(position.x-frame_width_, position.y-frame_height_, frame_width_*3, frame_height_*3), {0, 0}, 0, WHITE);
	auto end_time = std::chrono::steady_clock::now();
}

void Sprite::reset_animation()
{
	frame_number_ = 0;
	del_time_elapsed_ = std::chrono::nanoseconds{0};
}

Vector2 Sprite::frame_size() const
{
	return Vector2(frame_width_, frame_height_);
}
