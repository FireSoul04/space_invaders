#include "entity.hpp"

Entity::Entity(double x, double y, double speed, bool solid)
    : x(x), y(y), speed(speed), solid(solid), alive(true) {
}

Entity::Entity(double x, double y, bool solid)
    : x(x), y(y), speed(0.0), solid(solid), alive(true) {
}

void Entity::set_speed(double speed) {
    this->speed = speed;
}

bool Entity::is_colliding(std::shared_ptr<Entity>& e) {
    bool is_colliding = false;
    if (e->is_solid() && e.get() != this) {
        double offsetw = get_sprite()->w / 2;
        double offseth = get_sprite()->h;
        double e_offsetw = e->get_sprite()->w / 2;
        double e_offseth = e->get_sprite()->h;
        double x1 = x + offsetw;
        double x2 = e->X() + e_offsetw;
        double y1 = y + offseth;
        double y2 = e->Y() + e_offseth;
        if (SDL_abs(x2 - x1) < e_offsetw && 
            SDL_abs(y2 - y1) < e_offseth) {
            is_colliding = true;
        }
    }
    return is_colliding;
}

double Entity::get_velocity() {
    return velocity;
}

void Entity::set_velocity(double velocity) {
    this->velocity = velocity;
}

void Entity::reset_velocity() {
    velocity = 0.0;
}

double Entity::X() {
    return x;
}

double Entity::Y() {
    return y;
}

bool Entity::is_alive() {
    return alive;
}

bool Entity::is_solid() {
    return solid;
}

int Entity::get_id() {
    return id;
}

void Entity::set_id(int id) {
    this->id = id;
}

void Entity::update() {
    y += velocity * speed * core.get_delta_time();
    x += velocity * speed * core.get_delta_time();
}

void Entity::die() {
    alive = false;
}

void Entity::animate_death(SDL_Renderer *renderer) {
}

EntityClass Entity::get_class() {
    return ENTITY;
}

std::shared_ptr<SDL_Rect> Entity::get_sprite() {
    return nullptr;
}

SDL_Color Entity::get_color() {
    return { 0 };
}

bool operator==(std::shared_ptr<Entity> e, EntityClass c) {
    return e->get_class() == c;
}