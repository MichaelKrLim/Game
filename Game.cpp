#include "Game.hpp"

#include "Enemy.hpp"
#include "Player.hpp"
#include "rl/maths.hpp"
#include "rl/operator_overloads.hpp"

void Game::update(std::chrono::nanoseconds del_time)
{
	constexpr std::chrono::nanoseconds second = std::chrono::seconds{1};
	const auto seconds_passed = static_cast<float>(del_time.count())/second.count();
	
	camera_position += camera_velocity;
	const auto camera_displacement = player_position()-camera_position;
	camera_velocity = camera_velocity*0.7+(seconds_passed/2)*camera_displacement;
	for(auto& positioned_entity : entities_)
	{	
		auto del_position = positioned_entity.entity->update(del_time, *this, positioned_entity.position);
		Vector2 new_unvalidated_position = positioned_entity.position+del_position;
		if(sprite_can_move_to_without_collision(new_unvalidated_position, positioned_entity.entity->render_size(), positioned_entity.position))
		{
			positioned_entity.position = new_unvalidated_position;
		}
	}
}

bool Game::position_is_free(const Vector2& position, const Vector2& sprite_size) const
{
	return (current_map_.is_reachable(position, current_tile_set_) &&
			current_map_.is_reachable(position+Vector2{sprite_size.x,0}, current_tile_set_) &&
			current_map_.is_reachable(position+sprite_size, current_tile_set_) &&
			current_map_.is_reachable(position+Vector2{0,sprite_size.y}, current_tile_set_));
}

Game::Game()
{
	entities_.emplace_back(std::make_unique<Player>(), Vector2{80, 80});
	entities_.emplace_back(std::make_unique<Enemy>(), Vector2{100, 100});
}

void Game::render()
{
	ClearBackground(WHITE);
	auto offset = Vector2(GetScreenWidth(), GetScreenHeight())/2-camera_position;
	current_map_.render(current_tile_set_, offset);
	for(const auto& positioned_entity : entities_)
	{
		positioned_entity.entity->render(positioned_entity.position+offset);
	}
}

bool Game::sprite_can_move_to_without_collision(const Vector2& destination, const Vector2& sprite_size, const Vector2& starting_position)
{
	const auto step_size = std::min(sprite_size.x, sprite_size.y);
	for(int i{}; i<static_cast<int>(magnitude(destination-starting_position)/step_size); ++i)
	{
		if(!position_is_free(starting_position+i*(normalise(destination-starting_position)*step_size), sprite_size))
			return false;
	}

	if(!position_is_free(destination, sprite_size))
		return false;

	return true;
}
