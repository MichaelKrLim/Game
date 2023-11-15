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
		void render(const Tile_set& tile_set, Vector2 position);
		Map(std::initializer_list<std::filesystem::path> layer_paths);
	
	private:
	std::vector<Layer> layers{};
};

#endif // Map_hpp_INCLUDED

