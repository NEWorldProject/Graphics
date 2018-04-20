cmake_minimum_required(VERSION 3.9)
project(Graphics)

cmake_policy(SET CMP0003 NEW)

set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Set Build Output Path
set(PRODUCT_DIR ${CMAKE_BINARY_DIR}/Product-$<CONFIG>)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SDK/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PRODUCT_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PRODUCT_DIR})

if(UNIX)
    link_libraries(dl stdc++fs)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_DIR}/Build/CMake")

file(GLOB_RECURSE SRC_GRAP Source/*.*)
add_executable(Graphics ${SRC_GRAP})

# Optional IPO. Do not use IPO if it's not supported by compiler.
cmake_policy(SET CMP0069 NEW)
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    set_property(TARGET Graphics PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO is not supported: ${output}")
endif()

# Link libraries for NEWorld
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(GLEW REQUIRED)
set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_include_directories(Graphics PUBLIC Source)
target_include_directories(Graphics PRIVATE
        ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(Graphics Threads::Threads Common
        ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})