#include "wall.hpp"

Wall::Wall(double x, double y, std::shared_ptr<SDL_Rect>& sprite, SDL_Color color) : Entity(x, y, true) {
    this->sprite = sprite;
    sprite->x = (int)x;
    sprite->y = (int)y;
    this->color = color;
    life_points = 10;
}

Wall::~Wall() {
    sprite = nullptr;
}

void Wall::update() {
    
}

bool Wall::is_alive() {
    return life_points > 0;
}

void Wall::die() {
    life_points--;
    color.r += 16;
    color.b += 8;
    color.g -= 16;
    if (!is_alive()) {
        alive = false;
    }
}

std::shared_ptr<SDL_Rect> Wall::get_sprite() {
    return sprite;
}

SDL_Color Wall::get_color() {
    return color;
}

EntityClass Wall::get_class() {
    return WALL;
}