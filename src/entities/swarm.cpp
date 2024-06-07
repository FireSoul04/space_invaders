#include "swarm.hpp"

Swarm::Swarm(double x, double y, double speed, double shoot_frequency) : Entity(x, y, speed, false) {
    this->shoot_frequency = shoot_frequency;
    aliens = new std::shared_ptr<Alien>[core.max_aliens];
    init_x = x;
    init_y = y;
    velocity = 1.0;
    start_freq = shoot_frequency;
    start_speed = speed;
    width = core.get_entity_width() * (3 * core.max_aliens_per_row - 2);
    alive = false;
    reset_timer();
}

Swarm::~Swarm() {
    delete[] aliens;
}

void Swarm::restart() {
    x = init_x;
    y = init_y;
    aliens_alive = 0;
    velocity = 1.0;
    velocity_y = 0.0;
    reset_stats();
    reset_timer();
}

// Handles out of bounds
void Swarm::update() {
    if (left_alien() < core.get_bounds(LEFT) ||
        right_alien() + core.get_entity_width() > core.get_bounds(RIGHT)) {
        velocity *= -1.0;
        velocity_y = core.get_entity_height() * 2.0;
    } else {
        velocity_y = 0.0;
    }

    x += velocity * speed * core.get_delta_time();
    y += velocity_y;
}

std::shared_ptr<Projectile> Swarm::shoot() {
    std::shared_ptr<Projectile> p = nullptr;

    // Make random alien shoot
    if (is_ready_to_shoot() && aliens_alive > 0) {
        std::shared_ptr<Alien> random_alien = search_random_alien();
        p = random_alien->shoot();
    }

    return p;
}

void Swarm::add_alien(std::shared_ptr<Alien>& alien, int index) {
    alien->set_swarm_speed(&speed);
    alien->set_swarm_velocity(&velocity);
    alien->set_swarm_velocity_y(&velocity_y);
    aliens[index] = alien;
    aliens_alive++;
}

void Swarm::remove_alien(std::shared_ptr<Alien>& alien) {
    bool found = false;
    int index;
    for (int i = 0; i < core.max_aliens && !found; i++) {
        if (aliens[i] is alien) {
            found = true;
            index = i;
        }
    }

    if (found) {
        aliens_alive--;
        aliens[index] = nullptr;
    }
}

double Swarm::last_row_alien_height() {
    double last_y = 0.0;
    for (int i = 0; i < core.max_aliens; i++) {
        std::shared_ptr<Alien> a = aliens[i];
        if (a != nullptr && a->Y() >= last_y) {
            last_y = a->Y();
        }
    }
    return last_y;
}

void Swarm::double_stats() {
    shoot_frequency *= 0.9;
    speed *= 2;
}

double Swarm::get_speed() {
    return speed;
}

EntityClass Swarm::get_class() {
    return SWARM;
}

double Swarm::time_elapsed() {
    return (clock() - shoot_timer) / (double)CLOCKS_PER_SEC;
}

bool Swarm::is_ready_to_shoot() {
    bool is_ready = false;
    if (time_elapsed() >= shoot_frequency) {
        reset_timer();
        is_ready = true;
    }
    return is_ready;
}

void Swarm::reset_stats() {
    shoot_frequency = start_freq;
    speed = start_speed;
}

void Swarm::reset_timer() {
    shoot_timer = clock();
}

double Swarm::left_alien() {
    double left_x = core.get_width();
    for (int i = 0; i < core.max_aliens; i++) {
        std::shared_ptr<Alien> a = aliens[i];
        if (a != nullptr && a->X() <= left_x) {
            left_x = a->X();
        }
    }
    return left_x;
}

double Swarm::right_alien() {
    double right_x = 0.0;
    for (int i = 0; i < core.max_aliens; i++) {
        std::shared_ptr<Alien> a = aliens[i];
        if (a != nullptr && a->X() >= right_x) {
            right_x = a->X();
        }
    }
    return right_x;
}

std::shared_ptr<Alien> Swarm::search_random_alien() {
    std::shared_ptr<Alien> random_alien = nullptr;
    while (random_alien == nullptr) {
        int random = rand() % core.max_aliens_per_row;
        int row = 0;
        // Search the alien in the random column and in the last row alive
        for (int i = core.max_rows - 1; i >= 0 && random_alien == nullptr; i--) {
            row = core.max_aliens_per_row * i;
            random_alien = aliens[random + row];
        }
    }
    return random_alien;
}