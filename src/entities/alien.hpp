#pragma once

#include <SDL2/SDL.h>
#include <ctime>
#include <memory>

#include "../game_core/core.hpp"
#include "entity.hpp"
#include "projectile.hpp"

enum AlienType {
    CLOSE = 1,
    MID = 2,
    FAR = 3
};

class Alien : public Entity {
public:
    Alien(double x, double y, AlienType type, std::shared_ptr<SDL_Rect>& sprite, SDL_Color color);
    ~Alien();
    void update();
    std::shared_ptr<Projectile> shoot();
    void animate_death(SDL_Renderer *renderer);
    void set_swarm_speed(double *swarm_speed);
    void set_swarm_velocity(double *swarm_velocity);
    void set_swarm_velocity_y(double *swarm_velocity_y);
    AlienType get_type();
    std::shared_ptr<SDL_Rect> get_sprite();
    SDL_Color get_color();
    EntityClass get_class();

private:
    std::shared_ptr<SDL_Rect> sprite;
    SDL_Color color;
    AlienType type;
    double projectile_speed = 6000.0;
    double *swarm_speed;
    double *swarm_velocity;
    double *swarm_velocity_y;
};