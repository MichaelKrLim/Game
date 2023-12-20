#include "Player.hpp"

#include <cmath>

Vector2 Player::handle_movement(std::chrono::nanoseconds del_time)
{
	Vector2 movement_direction{0, 0};
	is_moving_ = true;
	constexpr std::chrono::nanoseconds second = std::chrono::seconds{1};
	const auto distance = pixels_per_second_*static_cast<double>(del_time.count())/second.count();
	if(IsKeyDown(KEY_W))
	{
		movement_direction.y-=1;
		current_animation_ = 1;
	}
	if(IsKeyDown(KEY_A))
	{
		movement_direction.x-=1;
		current_animation_ = 3;
	}
	if(IsKeyDown(KEY_S))
	{
		movement_direction.y+=1;
		current_animation_ = 0;
	}
	if(IsKeyDown(KEY_D))
	{
		movement_direction.x+=1;
		current_animation_ = 2;
	}
	const auto length = std::hypot(movement_direction.x, movement_direction.y);
	if(length == 0)
	{
		is_moving_ = false;
		sprite_.reset_animation();
		return {0,0};
	}

	const Vector2 normalised_direction{movement_direction.x/length, movement_direction.y/length};
	const Vector2 velocity(normalised_direction.x*distance, normalised_direction.y*distance);
	return velocity;
}

Vector2 Player::update(std::chrono::nanoseconds del_time)
{
	if(is_moving_)
		sprite_.update(del_time);
	return handle_movement(del_time);
}

void Player::render(Vector2 position)
{
	sprite_.render(current_animation_, position);
}

