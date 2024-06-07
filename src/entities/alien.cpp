#include "alien.hpp"

Alien::Alien(double x, double y, AlienType type, std::shared_ptr<SDL_Rect>& sprite, SDL_Color color) : Entity(x, y, true) {
    this->type = type;
    this->sprite = sprite;
    sprite->x = (int)x;
    sprite->y = (int)y;
    this->color = color;
}

Alien::~Alien() {
    core.add_score(10 * type);
    sprite = nullptr;
}

// Handles out of bounds
void Alien::update() {
    speed = *swarm_speed;
    velocity = *swarm_velocity;
    x += velocity * speed * core.get_delta_time();
    y += *swarm_velocity_y;
    sprite->x = (int)x;
    sprite->y = (int)y;
}

std::shared_ptr<Projectile> Alien::shoot() {
    std::shared_ptr<SDL_Rect> rect = std::make_shared<SDL_Rect>();
    rect->w = 5;
    rect->h = 10;
    double offset_x = (sprite->w / 2) - (rect->w / 2);
    double offset_y = sprite->h + core.get_entity_height();
    double x, y;

    x = this->x + offset_x;
    y = this->y + offset_y;

    return std::make_shared<Projectile>(x, y, projectile_speed, 0.1, rect, yellow);
}

void Alien::set_swarm_speed(double *swarm_speed) {
    this->swarm_speed = swarm_speed;
}

void Alien::set_swarm_velocity(double *swarm_velocity) {
    this->swarm_velocity = swarm_velocity;
}

void Alien::set_swarm_velocity_y(double *swarm_velocity_y) {
    this->swarm_velocity_y = swarm_velocity_y;
}

AlienType Alien::get_type() {
    return type;
}

std::shared_ptr<SDL_Rect> Alien::get_sprite() {
    return sprite;
}

SDL_Color Alien::get_color() {
    return color;
}

EntityClass Alien::get_class() {
    return ALIEN;
}

void Alien::animate_death(SDL_Renderer *renderer) {
    SDL_Rect rects[4];

    rects[0].w = 10;
    rects[0].h = 2;
    rects[0].x = x - core.get_entity_width();
    rects[0].x = y;

    rects[1].w = 10;
    rects[1].h = 2;
    rects[1].x = x + core.get_entity_width();
    rects[1].x = y;

    rects[2].w = 2;
    rects[2].h = 10;
    rects[2].x = x;
    rects[2].x = y - core.get_entity_width();

    rects[3].w = 2;
    rects[3].h = 10;
    rects[3].x = x;
    rects[3].x = y + core.get_entity_width();

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRects(renderer, rects, 4);
}