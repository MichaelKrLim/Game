#include "Map.hpp"

void Map::render(const Tile_set& tile_set, Vector2 position)
{
	for(int i{0}; i<layers.size(); ++i)
	{
		layers[i].render(tile_set, position);
	}
}

Map::Map(std::initializer_list<std::filesystem::path> layer_paths)
{
	for(const auto& layer_path : layer_paths)
	{
		layers.push_back(Layer{layer_path});
	}
}
