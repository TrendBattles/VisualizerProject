cmake_minimum_required(VERSION 3.15)
project(DSVisualizer)

# Use C++17 for modern data structure features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Define the output path
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build")

# 2. Find Raylib
# Since you installed it via pacman, CMake can find it easily
find_package(raylib REQUIRED)

# 3. Define your source files
# Add more files here as your project grows (e.g., LinkedList.cpp)
add_executable(${PROJECT_NAME} "src/main.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

# 4. Link the libraries
# We link raylib and the standard Windows graphics libraries
target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 winmm)