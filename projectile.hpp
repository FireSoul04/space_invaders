#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "core.hpp"
#include "entity.hpp"

class Projectile : public Entity {
public:
    Projectile(double x, double y, double speed, double velocity, SDL_Rect *sprite, SDL_Color color) : Entity(x, y, speed) {
        this->velocity = velocity;
        this->sprite = sprite;
        this->color = color;
        sprite->x = (int)x;
        sprite->y = (int)y;
    }

    ~Projectile() {
        delete sprite;
        sprite = nullptr;
    }

    void update() {
        y += velocity * speed * core->get_delta_time();
        sprite->y = (int)y;
    }

    SDL_Rect *get_sprite() {
        return sprite;
    }

    bool out_of_bounds() {
        return y >= core->get_bounds(BOTTOM) || y <= core->get_bounds(TOP);
    }

    SDL_Color get_color() {
        return color;
    }

    EntityClass get_class() {
        return PROJECTILE;
    }

private:
    SDL_Rect *sprite;
    SDL_Color color;
};

#endif