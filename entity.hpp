#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

enum EntityClass {
    ENTITY,
    SHIP,
    ALIEN,
    SWARM,
    WALL,
    PROJECTILE
};

class Entity {
public:
    Entity(double x, double y, double speed) {
        this->x = x;
        this->y = y;
        this->speed = speed;
        velocity = 0.0;
    }

    Entity(double x, double y) {
        this->x = x;
        this->y = y;
        speed = 0.0;
        velocity = 0.0;
    }

    void set_speed(double speed) {
        this->speed = speed;
    }

    bool is_colliding(Entity *e) {
        bool is_colliding = false;
        if (e != this) {
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

    double get_velocity() {
        return velocity;
    }

    void set_velocity(double velocity) {
        this->velocity = velocity;
    }

    void reset_velocity() {
        velocity = 0.0;
    }

    double X() {
        return x;
    }

    double Y() {
        return y;
    }

    int get_id() {
        return id;
    }

    void set_id(int id) {
        this->id = id;
    }

    virtual void update() {
        y += velocity * speed * core->get_delta_time();
        x += velocity * speed * core->get_delta_time();
    }

    virtual EntityClass get_class() {
        return ENTITY;
    }

    virtual SDL_Rect *get_sprite() {
        return nullptr;
    }

    virtual SDL_Color get_color() {
        return { 0 };
    }

protected:
    double x, y, velocity, speed;

private:
    int id;
};

#endif