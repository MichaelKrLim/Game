#ifndef Layer_hpp_INCLUDED
#define Layer_hpp_INCLUDED

#include "Tile_set.hpp"

#include <raylib.h>

#include <filesystem>
#include <vector>

class Layer
{
	public:
	Layer(const std::filesystem::path&);
	void render(const Tile_set&, const Vector2&);
	
	private:
	int row_num_{0};
	std::vector<int> map_;
};

#endif // Layer_hpp_INCLUDED

