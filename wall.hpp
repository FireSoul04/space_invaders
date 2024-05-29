#ifndef WALL_H
#define WALL_H

#include <SDL2/SDL.h>

#include "core.hpp"
#include "entity.hpp"

class Wall : public Entity {
public:
    Wall(double x, double y, SDL_Rect *sprite, SDL_Color color) : Entity(x, y) {
        this->sprite = sprite;
        sprite->x = (int)x;
        sprite->y = (int)y;
        this->color = color;
        life_points = 10;
    }

    ~Wall() {
        delete sprite;
        sprite = nullptr;
    }

    void update() {
        
    }

    bool is_alive() {
        return life_points > 0;
    }

    void hit() {
        life_points--;
        color.r += 16;
        color.b += 8;
        color.g -= 16;
    }

    SDL_Rect *get_sprite() {
        return sprite;
    }

    SDL_Color get_color() {
        return color;
    }

    EntityClass get_class() {
        return WALL;
    }

private:
    SDL_Rect *sprite;
    SDL_Color color;
    uint life_points;
};

#endif