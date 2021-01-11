cmake_minimum_required(VERSION 3.19)
project(touchegg_music_client)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCE_FILES RELATIVE ${PROJECT_SOURCE_DIR} src/*.h src/*.cpp)

add_executable(touchegg_music_client ${SOURCE_FILES})