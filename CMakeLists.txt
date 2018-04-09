cmake_minimum_required(VERSION 3.9)
project(Graphics)

set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_DIR ${CMAKE_SOURCE_DIR})

if(UNIX)
    link_libraries(dl stdc++fs)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_DIR}/Build/CMake")

file(GLOB_RECURSE SRC_GRAP Source/*.*)
add_executable(Graphics ${SRC_GRAP})

# Link libraries for NEWorld
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(GLEW REQUIRED)
set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_include_directories(Graphics PUBLIC
        ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(Graphics Threads::Threads
        ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})