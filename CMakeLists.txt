nw_project_prepare(CoreFeatures)
nwstd_add_executable(Graphics ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries for NEWorld
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
set(SDL2_BUILDING_LIBRARY ON)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_include_directories(Graphics PUBLIC Source)
target_include_directories(Graphics PRIVATE
        ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
target_link_libraries(Graphics ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})