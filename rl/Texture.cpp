#include "Texture.hpp"

namespace rl
{
	Texture::Texture(std::filesystem::path path)
	{
		texture_ = LoadTexture(path.c_str()); 
	}

	Texture::~Texture()
	{
		UnloadTexture(texture_);
	}
}
