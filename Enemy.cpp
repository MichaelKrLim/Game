#include "Enemy.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <unordered_set>

#include "rl/maths.hpp"
#include "rl/operator_overloads.hpp"

Vector2 Enemy::select_desired_position(Vector2 map_size, Vector2 enemy_position, Vector2 player_position)
{
	//A*
	// TODO fix enumerate_neighbours creating negative positions
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

	auto enumerate_neighbours = [](const auto& current_waypoint) -> std::vector<Vector2>
	{
		return std::vector<Vector2> 
		{
			{current_waypoint.position.x+1, current_waypoint.position.y},
			{current_waypoint.position.x-1, current_waypoint.position.y},
			{current_waypoint.position.x, current_waypoint.position.y+1},
			{current_waypoint.position.x, current_waypoint.position.y-1}
		};
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
	auto cmp = [](const auto& lhs, const auto& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	};
	auto hash = [](const auto& lhs)
	{
		return static_cast<std::size_t>(lhs.x + lhs.y) ^ static_cast<std::size_t>(lhs.y * lhs.x);
	};
	std::vector<bool> visited{};
	visited.resize(map_size.x*map_size.y, false);

	to_visit.push(Waypoint{0, enemy_position});
	while(!to_visit.empty())
	{
		auto position_to_index = [&](const auto& position)
		{
			int index = (position.x*map_size.x)+position.y;
			return index;
		};
		
		auto current_waypoint = to_visit.top();
		const int index = position_to_index(current_waypoint.position);
		to_visit.pop();
		visited[index] = true;

		if(distance(current_waypoint.position, player_position) < 20)
		{
			return *current_waypoint.first_step;
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
	return enemy_position;
}

Vector2 Enemy::update(std::chrono::nanoseconds del_time, Vector2 position, Vector2 map_size, Vector2 player_position)
{
	desired_position = select_desired_position(map_size, position, player_position);

	constexpr std::chrono::nanoseconds second = std::chrono::seconds{1};
	const auto travelable_distance = pixels_per_second_*static_cast<double>(del_time.count())/second.count();
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
