#include "Game.hpp"

#include "Enemy.hpp"
#include "Player.hpp"
#include "rl/maths.hpp"
#include "rl/operator_overloads.hpp"

void Game::update_camera(std::chrono::nanoseconds del_time)
{
	constexpr std::chrono::nanoseconds second = std::chrono::seconds{1};
	const auto seconds_passed = static_cast<float>(del_time.count())/second.count();

	camera_position_ += camera_velocity_;
	const auto camera_displacement = player_position()-camera_position_;
	camera_velocity_ = camera_velocity_*camera_velocity_dampening_multiplier_+(seconds_passed*camera_velocity_multiplier_)*camera_displacement;
	
	double scaled_screen_width = GetScreenWidth()/zoom_;
	double scaled_screen_height = GetScreenHeight()/zoom_;
	Vector2 max_distance_of_player_from_camera = Vector2(scaled_screen_width/2, scaled_screen_height/2)*camera_fraction_of_resolution_bound_;
	if(std::abs(player_position().x-camera_position_.x) > max_distance_of_player_from_camera.x)
		camera_position_.x = player_position().x + max_distance_of_player_from_camera.x*(player_position().x>camera_position_.x? -1 : 1);
	if(std::abs(player_position().y-camera_position_.y) > max_distance_of_player_from_camera.y)
		camera_position_.y = player_position().y + max_distance_of_player_from_camera.y*(player_position().y>camera_position_.y? -1 : 1);

	// ensure camera can not move outside of the map.
	if(camera_position_.x < scaled_screen_width/2) camera_position_.x = scaled_screen_width/2;
	if(camera_position_.y < scaled_screen_height/2) camera_position_.y = scaled_screen_height/2;
	int map_size_x = current_tile_set_.tile_size()*current_map_.size().x;
	int map_size_y = current_tile_set_.tile_size()*current_map_.size().y;
	if(camera_position_.x > map_size_x-scaled_screen_width/2)
		camera_position_.x = map_size_x-scaled_screen_width/2;
	if(camera_position_.y > map_size_y-scaled_screen_height/2)
		camera_position_.y = map_size_y-scaled_screen_height/2;
}

void Game::update(std::chrono::nanoseconds del_time)
{
	update_camera(del_time);
	
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

Game::~Game()
{
	UnloadRenderTexture(render_texture_);
}

void Game::render()
{
	ClearBackground(WHITE);
	auto offset = Vector2(GetScreenWidth()/zoom_, GetScreenHeight()/zoom_)/2-camera_position_;
	current_map_.render(current_tile_set_, offset);
	for(const auto& positioned_entity : entities_)
	{
		positioned_entity.entity->render(positioned_entity.position+offset);
	}

}

void Game::display()
{
	BeginTextureMode(render_texture_);

		render();

	EndTextureMode();
	
	BeginDrawing();

		const auto source_rectangle = Rectangle(0, 0, render_texture_.texture.width,-render_texture_.texture.height);
		const auto destination_rectangle = Rectangle(0, 0, render_texture_.texture.width*zoom_, render_texture_.texture.height*zoom_);
		DrawTexturePro(render_texture_.texture, source_rectangle, destination_rectangle, Vector2(0,0), 0, WHITE);

	EndDrawing();
	
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
