#pragma once

#include <SDL2/SDL.h>
#include <memory>

#include "../game_core/core.hpp"
#include "entity.hpp"
#include "projectile.hpp"

typedef unsigned int uint;

class Ship : public Entity {
public:
    Ship(double x, double y, double speed, double shoot_frequency, std::shared_ptr<SDL_Rect>& sprite, SDL_Color color);
    ~Ship();
    void update();
    std::shared_ptr<Projectile> shoot();
    void die();
    bool is_alive();
    uint get_lives();
    bool is_invincible();
    std::shared_ptr<SDL_Rect> get_sprite();
    SDL_Color get_color();
    EntityClass get_class();

private:
    double time_elapsed(clock_t timer);
    bool is_ready_to_shoot();
    void animate_iframe();
    void reset_shoot_timer();
    void reset_invincible();

private:
    std::shared_ptr<SDL_Rect> sprite;
    SDL_Color color;
    double shoot_frequency;
    clock_t invincible_timer;
    clock_t shoot_timer;
    bool invincible;
    uint lives = 3;
    int fade = 1;
    double max_iframe = 2.0;
    double projectile_speed = 6000.0;
};