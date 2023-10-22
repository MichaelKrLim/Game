#include "Tile_set.hpp"

Tile_set::Tile_set(std::filesystem::path path, int tile_size) :
	texture{LoadTexture(path.c_str())}, 
	tile_size{tile_size}
{

}

Tile_set::~Tile_set()
{
	UnloadTexture(texture);
}

void Tile_set::draw(unsigned tile_index, Vector2 position) const 
{
	const auto num_tiles_per_row = texture.width/tile_size;
	
	const auto row_index = tile_index/num_tiles_per_row,
	column_index = tile_index%num_tiles_per_row;
	
	const auto offset_x = column_index*tile_size,
	offset_y = row_index*tile_size;
	
	DrawTextureRec(texture, Rectangle(offset_x, offset_y, tile_size, tile_size), position, WHITE); 
}
