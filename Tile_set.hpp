#ifndef Tile_set_hpp_INCLUDED
#define Tile_set_hpp_INCLUDED

#include "rl/Texture.hpp"

#include <raylib.h>
#include <filesystem>

class Tile_set
{
	public:
		
	Tile_set(std::filesystem::path path, int tile_size);
	
	int tile_size() const;
	void draw(unsigned tile_index, Vector2 position) const;
	
	private:
	rl::Texture texture_;
	int tile_size_;
};

#endif
