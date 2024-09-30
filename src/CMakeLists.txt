cmake_minimum_required(VERSION 3.10)
project(space_invaders)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(FONT_PATH "${CMAKE_BINARY_DIR}/assets/font.ttf")
configure_file(config.h.in "${CMAKE_CURRENT_LIST_DIR}/config.h")

add_executable(${PROJECT_NAME}
    space_invaders.cpp
    game_core/core.cpp
    game_core/entitylist.cpp
    game_core/keybuffer.cpp
    entities/entity.cpp
    entities/projectile.cpp
    entities/alien.cpp
    entities/ship.cpp
    entities/swarm.cpp
    entities/wall.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::Main)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::TTF)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

