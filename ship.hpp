#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL.h>

#include "core.hpp"
#include "entity.hpp"
#include "projectile.hpp"

typedef unsigned int uint;

class Ship : public Entity {
public:
    Ship(double x, double y, double speed, double shoot_frequency, SDL_Rect *sprite, SDL_Color color) : Entity(x, y, speed) {
        this->sprite = sprite;
        this->speed = speed;
        sprite->x = (int)x;
        sprite->y = (int)y;
        this->color = color;
        this->shoot_frequency = shoot_frequency;
        lives = 3;
        projectile_speed = 3000.0;
        reset_timer();
    }

    ~Ship() {
        delete sprite;
        sprite = nullptr;
        core->set_game_over();
    }

    // Handles out of bounds
    void update() {
        if ((x >= core->get_bounds(LEFT) && velocity < 0.0) || 
            (x + sprite->w < core->get_bounds(RIGHT) && velocity > 0.0)) {
            x += velocity * speed * core->get_delta_time();
            sprite->x = (int)x;
        }
    }

    Projectile *shoot() {
        Projectile *p = nullptr;
        SDL_Rect *rect = new SDL_Rect();
        rect->w = 4;
        rect->h = 8;
        double offset_x = (sprite->w / 2) - (rect->w / 2) - 1;
        double offset_y = rect->h;
        double x, y;

        if (is_ready_to_shoot()) {
            x = this->x + offset_x;
            y = this->y - offset_y;
            p = new Projectile(x, y, projectile_speed, -0.1, rect, white);
        }

        return p;
    }

    SDL_Rect *get_sprite() {
        return sprite;
    }

    SDL_Color get_color() {
        return color;
    }

    EntityClass get_class() {
        return SHIP;
    }
    
private:
    double time_elapsed() {
        return (clock() - shoot_timer) / (double)CLOCKS_PER_SEC;
    }

    bool is_ready_to_shoot() {
        bool is_ready = false;
        if (time_elapsed() >= shoot_frequency) {
            reset_timer();
            is_ready = true;
        }
        return is_ready;
    }

    void reset_timer() {
        shoot_timer = clock();
    }

private:
    SDL_Rect *sprite;
    SDL_Color color;
    uint lives;
    clock_t shoot_timer;
    double shoot_frequency;
    double projectile_speed;
};

#endif