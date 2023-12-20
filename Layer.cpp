#include "Layer.hpp"

#include <fstream>

Layer::Layer(const std::filesystem::path& path)
{
	std::ifstream map_file(path);
	int current{};
	char ch;
	while(map_file.read(&ch,1))
	{
		if(std::isdigit(ch))
		{						
			current*=10;
			current+=ch-'0';
		}
		else if(ch == ',')
		{
			map_.push_back(current);
			current=0;
		}
		else if(ch == '\n')
		{
			++row_num_;
			map_.push_back(current);
			current=0;
		}
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
