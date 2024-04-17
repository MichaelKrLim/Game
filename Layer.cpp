#include "Layer.hpp"

#include <fstream>

Layer::Layer(const std::filesystem::path& path)
{
	std::ifstream map_file(path);
	for(int number{0}; map_file >> number;)
	{
		if(map_file.peek() != ',')
		{
			++row_num_;
		}
		else
			map_file.ignore(1);
		
		map_.push_back(number+1);
	}
}

void Layer::render(const Tile_set& tile_set,const Vector2& origin)
{
	const int column_num = map_.size()/row_num_;
	for(int i{0}; i<map_.size(); ++i)
	{
		const auto texture_index = map_[i];
		const int x_offset = i%column_num,
		y_offset = i/row_num_;
		tile_set.draw(texture_index, Vector2(x_offset*tile_set.tile_size()+origin.x, y_offset*tile_set.tile_size()+origin.y));

		//std::cout << "rendered at: x=" << i%column_num << " y=" << i/layer.row_num << "\n"; 
	}
}

int Layer::tile_id_at(int x, int y) const
{
	const int column_num = map_.size()/row_num_;
	const int index = y*column_num+x;
	return map_[index];
}

Vector2 Layer::size() const
{
	return Vector2(map_.size()/row_num_,row_num_);
}
