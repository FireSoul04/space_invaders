#pragma once

#include <SDL.h>
#include <memory>

#include "../game_core/core.hpp"

enum EntityClass {
    ENTITY,
    SHIP,
    SWARM,
    ALIEN,
    WALL,
    PROJECTILE
};

class Entity {
public:
    Entity(double x, double y, double speed, bool solid);
    Entity(double x, double y, bool solid);
    void set_speed(double speed);
    bool is_colliding(std::shared_ptr<Entity>& e);
    double get_velocity();
    void set_velocity(double velocity);
    void reset_velocity();
    double X();
    double Y();
    bool is_solid();
    int get_id();
    void set_id(int id);
    
    virtual void update();
    virtual bool is_alive();
    virtual void die();
    virtual void animate_death(SDL_Renderer *renderer);
    virtual EntityClass get_class();
    virtual std::shared_ptr<SDL_Rect> get_sprite();
    virtual SDL_Color get_color();
    
protected:
    double x, y, speed;
    double velocity = 0.0;
    bool solid, alive;

private:
    int id;
};

bool operator==(std::shared_ptr<Entity> e, EntityClass c);

#define is ==