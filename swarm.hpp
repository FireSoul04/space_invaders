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
    Swarm(double x, double y) : Entity(x, y) {
        aliens = new Alien *[core->max_aliens];
        init_x = x;
        init_y = y;
        shoot_frequency = 0;
        aliens_alive = 0;
        velocity = 1.0;
        width = core->get_entity_width() * (3 * core->max_aliens_per_row - 2);
        reset_timer();
    }

    ~Swarm() {
        delete[] aliens;
    }

    void print() {
        for (int i = 0; i < aliens_alive; i++) {
            std::cout << i << ", ";
        }
        std::cout << std::endl << "\033[1A";
    }

    void restart() {
        x = init_x;
        y = init_y;
        aliens_alive = 0;
        velocity = 1.0;
        reset_timer();
    }

    // Handles out of bounds
    void update() {
        if (x < core->get_bounds(LEFT) ||
            x + width > core->get_bounds(RIGHT)) {
            velocity *= -1.0;
        }

        x += velocity * speed * core->get_delta_time();
        y += (speed / 16) * core->get_delta_time();
        
        // Update all aliens alive in the swarm
        for (int i = 0; i < core->max_aliens; i++) {
            if (aliens[i] != nullptr) {
                aliens[i]->set_velocity(velocity);
            }
        }
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

    void set_shoot_frequency(double shoot_frequency) {
        this->shoot_frequency = shoot_frequency;
    }

    void set_speed(double speed) {
        this->speed = speed;
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

    void reset_timer() {
        shoot_timer = clock();
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
    double width;
    double init_x;
    double init_y;
};

#endif