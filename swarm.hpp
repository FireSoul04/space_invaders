#ifndef SWARM_H
#define SWARM_H

#include <ctime>
#include <SDL2/SDL.h>

#include "core.hpp"
#include "entity.hpp"
#include "alien.hpp"
#include "projectile.hpp"

class Swarm : public Entity {
public:
    Swarm(double x, double y, double speed, double shoot_frequency) : Entity(x, y, speed) {
        this->shoot_frequency = shoot_frequency;
        aliens = new Alien *[core->max_aliens];
        init_x = x;
        init_y = y;
        aliens_alive = 0;
        velocity = 1.0;
        velocity_y = 0.0;
        start_freq = shoot_frequency;
        start_speed = speed;
        width = core->get_entity_width() * (3 * core->max_aliens_per_row - 2);

        reset_timer();
    }

    ~Swarm() {
        delete[] aliens;
    }

    void restart() {
        x = init_x;
        y = init_y;
        aliens_alive = 0;
        velocity = 1.0;
        velocity_y = 0.0;
        reset_stats();
        reset_timer();
    }

    // Handles out of bounds
    void update() {
        if (left_alien() < core->get_bounds(LEFT) ||
            right_alien() + core->get_entity_width() > core->get_bounds(RIGHT)) {
            velocity *= -1.0;
            velocity_y = 5000.0;
        } else {
            velocity_y = 0.0;
        }

        x += velocity * speed * core->get_delta_time();
        y += velocity_y * speed * core->get_delta_time();
    }

    Projectile *shoot() {
        Projectile *p = nullptr;

        // Make random alien shoot
        if (is_ready_to_shoot() && aliens_alive > 0) {
            Alien *random_alien = search_random_alien();
            p = random_alien->shoot();
        }

        return p;
    }

    void add_alien(Alien *alien, int index) {
        alien->set_swarm_speed(&speed);
        alien->set_swarm_velocity(&velocity);
        alien->set_swarm_velocity_y(&velocity_y);
        aliens[index] = alien;
        aliens_alive++;
    }

    void remove_alien(Alien *alien) {
        bool found = false;
        int index;
        for (int i = 0; i < core->max_aliens && !found; i++) {
            if (aliens[i] == alien) {
                found = true;
                index = i;
            }
        }

        if (found) {
            aliens_alive--;
            aliens[index] = nullptr;
        }
    }

    double last_row_alien_height() {
        double last_y = 0.0;
        for (int i = 0; i < core->max_aliens; i++) {
            Alien *a = aliens[i];
            if (a != nullptr && a->Y() >= last_y) {
                last_y = a->Y();
            }
        }
        return last_y;
    }

    void double_stats() {
        shoot_frequency *= 0.9;
        speed *= 2;
    }

    double get_speed() {
        return speed;
    }

    EntityClass get_class() {
        return SWARM;
    }

private:
    double time_elapsed() {
        return (clock() - shoot_timer) / (double)CLOCKS_PER_SEC;
    }

    bool is_ready_to_shoot() {
        bool is_ready = false;
        if (time_elapsed() >= shoot_frequency) {
            reset_timer();
            is_ready = true;
        }
        return is_ready;
    }

    void reset_stats() {
        shoot_frequency = start_freq;
        speed = start_speed;
    }

    void reset_timer() {
        shoot_timer = clock();
    }

    double left_alien() {
        double left_x = core->get_width();
        for (int i = 0; i < core->max_aliens; i++) {
            Alien *a = aliens[i];
            if (a != nullptr && a->X() <= left_x) {
                left_x = a->X();
            }
        }
        return left_x;
    }

    double right_alien() {
        double right_x = 0.0;
        for (int i = 0; i < core->max_aliens; i++) {
            Alien *a = aliens[i];
            if (a != nullptr && a->X() >= right_x) {
                right_x = a->X();
            }
        }
        return right_x;
    }

    Alien *search_random_alien() {
        Alien *random_alien = nullptr;
        while (random_alien == nullptr) {
            int random = rand() % core->max_aliens_per_row;
            int row = 0;
            // Search the alien in the random column and in the last row alive
            for (int i = core->max_rows - 1; i >= 0 && random_alien == nullptr; i--) {
                row = core->max_aliens_per_row * i;
                random_alien = aliens[random + row];
            }
        }
        return random_alien;
    }

private:
    Alien **aliens;
    int aliens_alive;
    clock_t shoot_timer;
    double shoot_frequency;
    double velocity_y;
    double width;
    double init_x;
    double init_y;
    double start_freq;
    double start_speed;
};

#endif