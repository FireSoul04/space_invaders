#include "projectile.hpp"

Projectile::Projectile(double x, double y, double speed, double velocity, std::shared_ptr<SDL_Rect>& sprite, SDL_Color color) : Entity(x, y, speed, true) {
    this->velocity = velocity;
    this->sprite = sprite;
    this->color = color;
    sprite->x = (int)x;
    sprite->y = (int)y;
}

Projectile::~Projectile() {
    sprite = nullptr;
}

void Projectile::update() {
    y += velocity * speed * core.get_delta_time();
    sprite->y = (int)y;
}

std::shared_ptr<SDL_Rect> Projectile::get_sprite() {
    return sprite;
}

bool Projectile::out_of_bounds() {
    return y >= core.get_bounds(BOTTOM) || y <= core.get_bounds(TOP);
}

SDL_Color Projectile::get_color() {
    return color;
}

EntityClass Projectile::get_class() {
    return PROJECTILE;
}