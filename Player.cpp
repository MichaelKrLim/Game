#include "Player.hpp"
#include "Game.hpp"

#include <cmath>

#include "rl/operator_overloads.hpp"

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
		facing = Direction::left;
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
		facing = Direction::right;
	}
	const auto length = std::hypot(movement_direction.x, movement_direction.y);
	if(length == 0)
	{
		is_moving_ = false;
		sprite_.reset_animation();
		return {0, 0};
	}

	const Vector2 normalised_direction = movement_direction/length;
	const Vector2 velocity = normalised_direction*distance;
	return velocity;
}

Vector2 Player::update(std::chrono::nanoseconds del_time, Game_view game, const Vector2& position)
{
	using namespace std::chrono_literals;

	time_elapsed_since_last_attack_ += del_time;
	if(time_elapsed_since_last_attack_ > 400ms)
		is_attacking_ = false;
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && is_armed_ && !is_attacking_ && time_elapsed_since_last_attack_ > 500ms)
	{
		is_attacking_= true;
		time_elapsed_since_last_attack_ = 0ns;
	}
	if(IsKeyPressed(KEY_F))
		is_armed_ = !is_armed_;

	const auto colliding_entites_ids = game.collisions_with(Rectangle{position.x, position.y, render_size().x, render_size().y});
	ISHITTINGANYTHING = colliding_entites_ids.size() > 1;

	if(is_moving_)
		sprite_.update(del_time);
	return handle_movement(del_time);
}

void Player::render(Vector2 position)
{	
	sprite_.render(current_animation_, position);
	if(is_armed_)
	{
		int offset = facing == Direction::left ? -100 : 100;
		DrawRectangle(position.x+offset, position.y, 50, 50, ISHITTINGANYTHING ? ORANGE : is_attacking_? RED : DARKBLUE);	
	}
}

Vector2 Player::render_size() const
{
	return sprite_.frame_size();
}
