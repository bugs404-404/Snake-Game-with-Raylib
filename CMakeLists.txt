cmake_minimum_required(VERSION 3.5)
project(SnakeGame)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include the path to Raylib's headers
include_directories("C:/raylib/include")

# Link to Raylib's libraries
link_directories("C:/raylib/lib")

# Add the source file for the game
add_executable(SnakeGame main.cpp)

# Link Raylib and additional required libraries for Windows
target_link_libraries(SnakeGame raylib opengl32 gdi32 winmm)
