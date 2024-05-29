#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <ctime>

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
        start_iframe = (double)clock();
        max_iframe = 0.2;
        invincible = true;
    }

    ~Projectile() {
        delete sprite;
        sprite = nullptr;
    }

    void update() {
        y += velocity * speed * core->get_delta_time();
        sprite->y = (int)y;

        double end_iframe = time_elapsed();
        if (end_iframe >= max_iframe) {
            invincible = false;
        }
    }

    bool is_invincible() {
        return invincible;
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
    double time_elapsed() {
        return (clock() - start_iframe) / (double)CLOCKS_PER_SEC;
    }

private:
    SDL_Rect *sprite;
    SDL_Color color;
    bool invincible;
    clock_t start_iframe;
    double max_iframe;
};

#endif