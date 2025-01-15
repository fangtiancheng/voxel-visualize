cmake_minimum_required(VERSION 3.19)
project(voxel_visualize)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rdparty)
include_directories(src/)
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/voxel.cpp
        src/voxel.hpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC cnpy)
