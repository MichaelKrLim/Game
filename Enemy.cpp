#include "Enemy.hpp"
#include "Game.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <unordered_set>

#include "rl/maths.hpp"
#include "rl/operator_overloads.hpp"

std::optional<Vector2> Enemy::select_target_tile(Vector2 enemy_position, Vector2 player_position, const Game_view game)
{
	//A*
	struct Waypoint
	{
		int distance_from_start{};
		Vector2 position{};
		std::optional<Vector2> first_step;
		int estimated_distance_to_target{};
		
		bool operator < (const Waypoint& other) const
		{
			if(other.distance_from_start + other.estimated_distance_to_target == distance_from_start + estimated_distance_to_target)
				return other.estimated_distance_to_target < estimated_distance_to_target;
			return other.distance_from_start + other.estimated_distance_to_target < distance_from_start + estimated_distance_to_target;
		}
	};

	const Vector2 map_size_in_tiles = game.map_size_in_tiles();
	auto enumerate_neighbours = [&game, this, &map_size_in_tiles](const auto& current_waypoint) -> std::vector<Vector2>
	{
		std::vector<Vector2> unvalidated_neighbours =
		{
			{current_waypoint.position.x+1, current_waypoint.position.y},
			{current_waypoint.position.x-1, current_waypoint.position.y},
			{current_waypoint.position.x, current_waypoint.position.y+1},
			{current_waypoint.position.x, current_waypoint.position.y-1}
		};
		std::vector<Vector2> valid_neighbours{};
		for(const auto& position : unvalidated_neighbours)
		{
			const auto map_position = position*game.tile_size_in_pixels();
			const bool is_on_map = CheckCollisionPointRec(position, Rectangle(0, 0, map_size_in_tiles.x, map_size_in_tiles.y));
			if(is_on_map && game.position_is_free(map_position, render_size()))
				valid_neighbours.push_back(position);
		}
		return valid_neighbours;
	};

	auto enumerate_neighbouring_waypoints = [&](const auto& current_waypoint) -> std::vector<Waypoint>
	{
		std::vector<Waypoint> neighbouring_waypoints{};
		std::ranges::transform(enumerate_neighbours(current_waypoint), std::back_inserter(neighbouring_waypoints), [&](Vector2 neighbour)
		{
			const auto first_step = current_waypoint.first_step ? *current_waypoint.first_step : neighbour;
			const auto estimated_distance = std::abs(neighbour.x-player_position.x)+std::abs(neighbour.y-player_position.y);
			return Waypoint(current_waypoint.distance_from_start+1, neighbour, first_step, estimated_distance);
		});
		return neighbouring_waypoints;
	};

	std::priority_queue<Waypoint> to_visit{};
	std::vector<bool> visited{};
	visited.resize(map_size_in_tiles.x*map_size_in_tiles.y, false);

	to_visit.push(Waypoint{0, enemy_position});
	while(!to_visit.empty())
	{
		auto position_to_index = [&](const auto& position)
		{
			int index = (position.x*map_size_in_tiles.y)+position.y;
			return index;
		};
		
		auto current_waypoint = to_visit.top();
		const int index = position_to_index(current_waypoint.position);
		to_visit.pop();
		visited[index] = true;

		const auto endpoints = {Vector2(current_waypoint.position.x, current_waypoint.position.y+1)};
		for(const auto& endpoint : endpoints)
		{
			if(endpoint == player_position)
			{
				return *current_waypoint.first_step;
			}
		}

		for(const auto& neighbour : enumerate_neighbouring_waypoints(current_waypoint))
		{
			const int neighbour_index = position_to_index(neighbour.position);
			if(!visited[neighbour_index])
			{
				to_visit.push(neighbour);
				visited[neighbour_index] = true;
			}
		}
	}
	return std::nullopt;
}

Vector2 Enemy::update(std::chrono::nanoseconds del_time, Game_view game, const Vector2& position)
{
	const int tile_size = game.tile_size_in_pixels();	
	//if(distance(position + Vector2(0, tile_size), game.player_position()) < tile_size)
		return {0, 0};
	const Vector2 map_tile_size = game.map_size_in_tiles();
	const auto player_tile_position = Vector2(std::floor(game.player_position().x/tile_size), std::floor(game.player_position().y/tile_size));
	const auto enemy_tile_position = Vector2(std::floor(position.x/tile_size), std::floor(position.y/tile_size));
	const auto desired_tile = select_target_tile(enemy_tile_position, player_tile_position, game);
	if(!desired_tile)
		return {0, 0};

	const auto desired_position = *desired_tile*tile_size;
	constexpr std::chrono::nanoseconds second = std::chrono::seconds{1};
	const auto travelable_distance = std::min(magnitude(desired_position-position), pixels_per_second_*static_cast<float>(del_time.count())/second.count());
	const Vector2 normalised_vector = normalise(desired_position-position);
	
	if(is_moving_)
		sprite_.update(del_time);
	return travelable_distance*normalised_vector;
}

void Enemy::render(Vector2 position)
{
	sprite_.render(current_animation_, position);
}

Vector2 Enemy::render_size() const
{
	return sprite_.frame_size();
}
