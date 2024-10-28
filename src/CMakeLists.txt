cmake_minimum_required(VERSION 3.10)
project(space_invaders)

file(GLOB SOURCE_FILES
     game_core/*.hpp game_core/*.cpp
     entities/*.hpp entities/*.cpp
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

if(WIN32)
	find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
	find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
	find_package(SDL2_ttf REQUIRED CONFIG REQUIRED COMPONENTS SDL2_ttf)
else()
	find_package(SDL2 REQUIRED)
	find_package(SDL2_ttf REQUIRED)
endif()

set(FONT_PATH "${CMAKE_BINARY_DIR}/assets/font.ttf")
configure_file(config.h.in "${CMAKE_CURRENT_LIST_DIR}/config.h")

add_executable(${PROJECT_NAME}
    space_invaders.h
    space_invaders.cpp
    ${SOURCE_FILES}
)

if(TARGET SDL2::SDL2main)
	target_link_libraries(space_invaders PRIVATE SDL2::SDL2)
	target_link_libraries(space_invaders PRIVATE SDL2::SDL2main)
	target_link_libraries(space_invaders PRIVATE SDL2_ttf::SDL2_ttf)
else()
	target_link_libraries(space_invaders PRIVATE SDL2)
	target_link_libraries(space_invaders PRIVATE SDL2::TTF)
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

