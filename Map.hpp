#ifndef Map_hpp_INCLUDED
#define Map_hpp_INCLUDED

#include "Layer.hpp"
#include "Tile_set.hpp"

#include <raylib.h>

#include <filesystem>
#include <vector> 

class Map
{
	public:

	Map(std::initializer_list<std::filesystem::path> layer_paths, std::filesystem::path collision_layer);

	void render(const Tile_set& tile_set, Vector2 position);
	bool is_reachable(Vector2 position, const Tile_set& tile_set) const;
	Vector2 size() const;

	private:

	std::vector<Layer> layers{};
	Layer collision_layer_;
};

#endif // Map_hpp_INCLUDED

