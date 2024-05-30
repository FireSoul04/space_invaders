#ifndef ALIEN_H
#define ALIEN_H

#include <ctime>
#include <SDL2/SDL.h>

#include "core.hpp"
#include "entity.hpp"
#include "projectile.hpp"

enum AlienType {
    CLOSE = 1,
    MID = 2,
    FAR = 3
};

class Alien : public Entity {
public:
    Alien(double x, double y, AlienType type, SDL_Rect *sprite, SDL_Color color) : Entity(x, y) {
        this->type = type;
        this->sprite = sprite;
        sprite->x = (int)x;
        sprite->y = (int)y;
        this->color = color;
        velocity = 0.0;
        projectile_speed = 3000.0;
    }

    ~Alien() {
        core->add_score(10 * type);
        delete sprite;
        sprite = nullptr;
    }

    // Handles out of bounds
    void update() {
        speed = *swarm_speed;
        velocity = *swarm_velocity;
        x += velocity * speed * core->get_delta_time();
        y += *swarm_velocity_y * core->get_delta_time();
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

    void set_swarm_speed(double *swarm_speed) {
        this->swarm_speed = swarm_speed;
    }

    void set_swarm_velocity(double *swarm_velocity) {
        this->swarm_velocity = swarm_velocity;
    }

    void set_swarm_velocity_y(double *swarm_velocity_y) {
        this->swarm_velocity_y = swarm_velocity_y;
    }

    AlienType get_type() {
        return type;
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
    AlienType type;
    double projectile_speed;
    double *swarm_speed;
    double *swarm_velocity;
    double *swarm_velocity_y;
};

#endif