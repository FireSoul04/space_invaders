#pragma once

#include <memory>

#include "../game_core/core.hpp"
#include "entity.hpp"

class Projectile : public Entity {
public:
    Projectile(double x, double y, double speed, double velocity, std::shared_ptr<SDL_Rect>& sprite, SDL_Color color);
    ~Projectile();
    void update();
    std::shared_ptr<SDL_Rect> get_sprite();
    bool out_of_bounds();
    SDL_Color get_color();
    EntityClass get_class();

private:
    std::shared_ptr<SDL_Rect> sprite;
    SDL_Color color;
};