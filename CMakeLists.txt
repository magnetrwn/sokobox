cmake_minimum_required(VERSION 3.13.0)
project(sokobox)

add_definitions(-DSETTINGS_FILE="sokobox.ini")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/lib/raylib/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/raylib/build)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/inipp)

add_compile_options(-Wall -Wextra -Wpedantic)

target_link_libraries(sokobox PRIVATE "${CMAKE_SOURCE_DIR}/lib/raylib/build/raylib/libraylib.a")