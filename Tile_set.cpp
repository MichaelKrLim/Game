#include "Tile_set.hpp"

Tile_set::Tile_set(std::filesystem::path path, int tile_size) :
	texture_{LoadTexture(path.c_str())}, 
	tile_size_{tile_size}
{

}

Tile_set::~Tile_set()
{
	UnloadTexture(texture_);
}

void Tile_set::draw(unsigned tile_index, Vector2 position) const 
{
	const auto num_tiles_per_row = texture_.width/tile_size_;
	
	const auto row_index = tile_index/num_tiles_per_row,
	column_index = tile_index%num_tiles_per_row;
	
	const auto offset_x = column_index*tile_size_,
	offset_y = row_index*tile_size_;
	
	DrawTextureRec(texture_, Rectangle(offset_x, offset_y, tile_size_, tile_size_), position, WHITE);
}

int Tile_set::tile_size() const
{
	return tile_size_;
}
