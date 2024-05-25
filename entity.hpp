#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

enum EntityClass {
    ENTITY,
    SHIP,
    ALIEN,
    SWARM,
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

    bool is_colliding(Entity *e) {
        bool is_colliding = false;
        if (e != this) {
            double x1 = this->x + get_sprite()->w / 2;
            double x2 = e->X() + e->get_sprite()->w / 2;
            if (SDL_abs(x2 - x1) < 10.0 && SDL_abs(y - e->Y()) < 1.0) {
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