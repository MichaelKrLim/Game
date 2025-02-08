#ifndef Resource_buffer_hpp_INCLUDED
#define Resource_buffer_hpp_INCLUDED

#include "rl/Texture.hpp"

#include <raylib.h>

#include <filesystem>
#include <memory>
#include <unordered_map>

class Resource_buffer
{
	public:
	std::weak_ptr<rl::Texture> get_texture(std::filesystem::path path) const
	{
		if(!texture_buffer_.contains(path))
		{
			texture_buffer_[path] = std::make_shared<rl::Texture>(path);
		}
		return std::weak_ptr<rl::Texture>(texture_buffer_[path]);
	}

	private:
	mutable std::unordered_map<std::filesystem::path, std::shared_ptr<rl::Texture>> texture_buffer_;
};

#endif // Resource_buffer_hpp_INCLUDED
