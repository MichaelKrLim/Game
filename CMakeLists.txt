# always required
cmake_minimum_required(VERSION 3.5.0)
project(game VERSION 0.0.1)

# for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# copies assets to build dir for runtime
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# for windows
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# for raylib
include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/4.5.0.tar.gz
    USES_TERMINAL_DOWNLOAD TRUE
    GIT_PROGRESS TRUE
    DOWNLOAD_NO_EXTRACT FALSE
)
FetchContent_MakeAvailable(raylib)

# code dependant
add_executable(
	game
	main.cpp
	Layer.cpp Layer.hpp 
	Tile_set.cpp Tile_set.hpp
	Map.cpp Map.hpp
	Sprite.cpp Sprite.hpp
	Player.cpp Player.hpp
	Enemy.cpp Enemy.hpp
	Entity.hpp

	rl/Texture.cpp rl/Texture.hpp
	rl/operator_overloads.hpp
	rl/maths.hpp
)

#linking & sets cpp standered
target_link_libraries(game PRIVATE raylib)
set_property(TARGET game PROPERTY CXX_STANDARD 20)
