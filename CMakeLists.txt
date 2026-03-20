cmake_minimum_required(VERSION 3.15)
project(DSVisualizer)

# Use C++17 for modern data structure features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the output path
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build")

# Find Raylib
find_package(raylib REQUIRED)

# Define your source files
add_executable(${PROJECT_NAME} "src/main.cpp")

# Define the include path
target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/include
)

# Core Managers
target_sources(${PROJECT_NAME} PRIVATE
	"src/Core/AppLoop.cpp"
	"src/Core/EventManager.cpp"
	"src/Core/StateManager.cpp"
)

# Data Structures
target_sources(${PROJECT_NAME} PRIVATE
	"src/DS/IDataStructure.cpp"
	"src/DS/Tree/AVL.cpp"
)

# Graphics
target_sources(${PROJECT_NAME} PRIVATE
	"src/Graphics/Helper.cpp"
	"src/Graphics/RendererCommon.cpp"
	"src/Graphics/UIManager.cpp"
)

# Link the libraries
# We link raylib and the standard Windows graphics libraries
target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 winmm)