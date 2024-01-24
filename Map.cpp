#include "Map.hpp"

void Map::render(const Tile_set& tile_set, Vector2 position)
{
	for(int i{0}; i<layers.size(); ++i)
	{
		layers[i].render(tile_set, position);
	}
}

Map::Map(std::initializer_list<std::filesystem::path> layer_paths, std::filesystem::path collision_layer) :
	collision_layer_{collision_layer}
{
	for(const auto& layer_path : layer_paths)
	{
		layers.push_back(Layer{layer_path});
	}
}

bool Map::is_reachable(Vector2 position, const Tile_set& tile_set) const
{
	const int tile_id_x = position.x/tile_set.tile_size(),
	tile_id_y = position.y/tile_set.tile_size();
	if(collision_layer_.tile_id_at(tile_id_x, tile_id_y) == 0)
	{
		return true;
	}
	return false;
}

Vector2 Map::size() const
{
	return collision_layer_.size();
}
