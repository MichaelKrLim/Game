#ifndef rl_Texture_hpp_INCLUDED
#define rl_Texture_hpp_INCLUDED

#include "raylib.h"

#include <filesystem>

namespace rl
{
	class Texture
	{
		public:
		Texture(std::filesystem::path path);
		~Texture();

		operator ::Texture() const { return texture_; }

		auto width() const { return texture_.width; }
		auto height() const { return texture_.height; }
		
		private:
		::Texture texture_;

		Texture(const Texture&) = delete;
		Texture& operator= (const Texture&) = delete;
	};
}

#endif // rl_Texture_hpp_INCLUDED

