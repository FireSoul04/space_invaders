#include "core.hpp"

Core::Core()
: time_start(system_clock::now())
 {

}

void Core::init(uint width, uint height, uint max_aliens_per_row, uint max_rows)
{
    this->width = width;
    this->height = height;
    this->max_aliens_per_row = max_aliens_per_row;
    this->max_rows = max_rows;
    max_aliens = max_aliens_per_row * max_rows;
    game_over_threshold = height * 5 / 8;
    bounds[LEFT] = entity_width * 2;
    bounds[RIGHT] = width - bounds[LEFT];
    bounds[TOP] = 0.0;
    bounds[BOTTOM] = height;
    reset_score();
}

void Core::set_game_over() {
    game_over = true;
}

void Core::reset_game_over() {
    game_over = false;
}

bool Core::is_game_over() {
    return game_over;
}

uint Core::get_game_over_threshold() {
    return game_over_threshold;
}

uint Core::get_width() {
    return width;
}

uint Core::get_height() {
    return height;
}

int Core::get_entity_width() {
    return entity_width;
}

int Core::get_entity_height() {
    return entity_height;
}

void Core::reset_score() {
    score = 0;
}

void Core::add_score(uint score) {
    this->score += score;
}

uint Core::get_score() {
    return score;
}

double Core::get_fps() {
    return fps;
}

double Core::get_delta_time() {
    return delta_time;
}

void Core::update_frame() {
    static time_point<system_clock> last_frame_time = system_clock::now();
    total_frames++;
    total_seconds = duration_cast<seconds>(system_clock::now() - time_start).count();
    delta_time = (duration_cast<nanoseconds>(system_clock::now() - last_frame_time).count()) / 1000000000.0;
    fps = total_frames / total_seconds;
    while (delta_time < 1) {
        delta_time += 0.1;
    }
    last_frame_time = system_clock::now();
}

double Core::get_bounds(Bounds direction) {
    return bounds[direction];
}

Core core;