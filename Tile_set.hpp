#ifndef Tile_set_hpp_INCLUDED
#define Tile_set_hpp_INCLUDED

#include <raylib.h>
#include <filesystem>

class Tile_set
{
	public:
	// constructors/destructor
	Tile_set(std::filesystem::path path, int tile_size);
	Tile_set(const Tile_set&) = delete;
	~Tile_set();
	// operators
	Tile_set& operator=(const Tile_set&) = delete;	
	// public functions
	int tile_size() const;
	void draw(unsigned tile_index, Vector2 position) const;
	
	private:
	Texture texture_;
	int tile_size_;
};

#endif
