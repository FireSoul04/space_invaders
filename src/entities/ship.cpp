#include "ship.hpp"

Ship::Ship(double x, double y, double speed, double shoot_frequency, std::shared_ptr<SDL_Rect>& sprite, SDL_Color color) : Entity(x, y, speed, true) {
    this->sprite = sprite;
    this->color = color;
    this->shoot_frequency = shoot_frequency;
    sprite->x = (int)x;
    sprite->y = (int)y;
    reset_shoot_timer();
    reset_invincible();
}

Ship::~Ship() {
    sprite = nullptr;
    core.set_game_over();
}

// Handles out of bounds
void Ship::update() {
    if ((x >= core.get_bounds(LEFT) && velocity < 0.0) || 
        (x + sprite->w < core.get_bounds(RIGHT) && velocity > 0.0)) {
        x += velocity * speed * core.get_delta_time();
        sprite->x = (int)x;
    }

    double end_iframe = time_elapsed(invincible_timer);
    if (end_iframe >= max_iframe) {
        reset_invincible();
    }
    animate_iframe();
}

std::shared_ptr<Projectile> Ship::shoot() {
    std::shared_ptr<Projectile> p = nullptr;
    std::shared_ptr<SDL_Rect> rect (new SDL_Rect());
    rect->w = 5;
    rect->h = 10;
    double offset_x = (sprite->w / 2) - (rect->w / 2) - 1;
    double offset_y = rect->h;
    double x, y;

    if (is_ready_to_shoot()) {
        x = this->x + offset_x;
        y = this->y - offset_y;
        p = std::make_shared<Projectile>(x, y, projectile_speed, -0.1, rect, white);
    }

    return p;
}

void Ship::die() {
    alive = invincible;
    if (!invincible) {
        lives--;
        invincible = true;
    }
}

bool Ship::is_alive() {
    return lives > 0;
}

uint Ship::get_lives() {
    return lives;
}

bool Ship::is_invincible() {
    return invincible;
}

std::shared_ptr<SDL_Rect> Ship::get_sprite() {
    return sprite;
}

SDL_Color Ship::get_color() {
    return color;
}

EntityClass Ship::get_class() {
    return SHIP;
}

double Ship::time_elapsed(clock_t timer) {
    return (clock() - timer) / (double)CLOCKS_PER_SEC;
}

bool Ship::is_ready_to_shoot() {
    bool is_ready = false;
    if (time_elapsed(shoot_timer) >= shoot_frequency) {
        reset_shoot_timer();
        is_ready = true;
    }
    return is_ready;
}

void Ship::animate_iframe() {
    if (invincible) {
        if (color.g >= 250) {
            fade = -1;
        } else if (color.g == 0) {
            fade = 1;
        }
        color.g += 750 * fade * core.get_delta_time();
    } else {
        color.g = 255;
    }
}

void Ship::reset_shoot_timer() {
    shoot_timer = clock();
}

void Ship::reset_invincible() {
    invincible = false;
    invincible_timer = clock();
}