#pragma once

#include <cstdlib>
#include <ctime>
#include <SDL.h>

#include "../game_core/core.hpp"
#include "entity.hpp"
#include "alien.hpp"
#include "projectile.hpp"

class Swarm : public Entity {
public:
    Swarm(double x, double y, double speed, double shoot_frequency);
    ~Swarm();

    void restart();
    void update();
    std::shared_ptr<Projectile> shoot();
    void add_alien(std::shared_ptr<Alien>& alien, int index);
    void remove_alien(std::shared_ptr<Alien>& alien);
    double last_row_alien_height();
    void double_stats();
    double get_speed();
    EntityClass get_class();

private:
    double time_elapsed();
    bool is_ready_to_shoot();
    void reset_stats();
    void reset_timer();
    double left_alien();
    double right_alien();
    std::shared_ptr<Alien> search_random_alien();

private:
    std::shared_ptr<Alien> *aliens;
    clock_t shoot_timer;
    int aliens_alive = 0;
    double velocity_y = 0.0;
    double shoot_frequency;
    double width;
    double init_x;
    double init_y;
    double start_freq;
    double start_speed;
};