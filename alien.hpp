#ifndef ALIEN_H
#define ALIEN_H

#include <ctime>
#include <SDL2/SDL.h>

#include "core.hpp"
#include "entity.hpp"
#include "projectile.hpp"

class Alien : public Entity {
public:
    Alien(double x, double y, double speed, SDL_Rect *sprite, SDL_Color color) : Entity(x, y, speed) {
        this->sprite = sprite;
        this->speed = speed;
        sprite->x = (int)x;
        sprite->y = (int)y;
        this->color = color;
        velocity = 0.0;
        projectile_speed = 3000.0;
    }

    ~Alien() {
        core->add_score(100);
        delete sprite;
        sprite = nullptr;
    }

    // Handles out of bounds
    void update() {
        x += velocity * speed * core->get_delta_time();
        y += (speed / 16) * core->get_delta_time();
        sprite->x = (int)x;
        sprite->y = (int)y;
    }

    Projectile *shoot() {
        SDL_Rect *rect = new SDL_Rect();
        rect->w = 5;
        rect->h = 10;
        double offset_x = (sprite->w / 2) - (rect->w / 2);
        double offset_y = sprite->h + core->get_entity_height();
        double x, y;

        x = this->x + offset_x;
        y = this->y + offset_y;

        return new Projectile(x, y, projectile_speed, 0.1, rect, yellow);
    }

    SDL_Rect *get_sprite() {
        return sprite;
    }

    SDL_Color get_color() {
        return color;
    }

    EntityClass get_class() {
        return ALIEN;
    }

private:
    SDL_Rect *sprite;
    SDL_Color color;
    double projectile_speed;
};

#endif