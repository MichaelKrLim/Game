#include "Enemy.hpp"

#include <cmath>
#include <random>

#include "rl/maths.hpp"
#include "rl/operator_overloads.hpp"

Vector2 Enemy::select_desired_position(Vector2 map_size)
{
	static std::random_device true_random_number_generator;
	static std::mt19937_64 mt(true_random_number_generator());
	std::uniform_real_distribution<float> x_distance_distribution(0,map_size.x);
	std::uniform_real_distribution<float> y_distance_distribution(0,map_size.x);
	
	int x_random_position = x_distance_distribution(mt);
	int y_random_position = y_distance_distribution(mt);

	return Vector2{x_random_position, y_random_position};
}

Vector2 Enemy::update(std::chrono::nanoseconds del_time, Vector2 position, Vector2 map_size)
{
	if(magnitude(desired_position-position) <= 1) 
		desired_position = select_desired_position(map_size);

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
