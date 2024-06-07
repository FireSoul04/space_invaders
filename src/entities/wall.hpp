#pragma once

#include <SDL.h>
#include <memory>

#include "entity.hpp"

class Wall : public Entity {
public:
    Wall(double x, double y, std::shared_ptr<SDL_Rect>& sprite, SDL_Color color);
    ~Wall();
    void update();
    bool is_alive();
    void die();
    std::shared_ptr<SDL_Rect> get_sprite();
    SDL_Color get_color();
    EntityClass get_class();

private:
    std::shared_ptr<SDL_Rect> sprite;
    SDL_Color color;
    uint life_points;
};