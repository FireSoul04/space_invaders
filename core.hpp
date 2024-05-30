#ifndef CORE_H
#define CORE_H

#include <ctime>
#include <SDL2/SDL.h>

#define DIRECTIONS 4

typedef unsigned int uint;

const SDL_Color black = { 0, 0, 0, 255 };
const SDL_Color white = { 255, 255, 255, 255 };
const SDL_Color green = { 0, 255, 0, 255 };
const SDL_Color yellow = { 255, 255, 0, 255 };

enum Bounds {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

class Core {
public:
    Core(uint width, uint height, uint max_aliens_per_row, uint max_rows) {
        this->width = width;
        this->height = height;
        this->max_aliens_per_row = max_aliens_per_row;
        this->max_rows = max_rows;
        max_aliens = max_aliens_per_row * max_rows;
        entity_width = 20.0;
        entity_height = 10.0;
        game_over_threshold = height * 3 / 8;
        bounds[LEFT] = entity_width * 2;
        bounds[RIGHT] = width - bounds[LEFT];
        bounds[TOP] = 0.0;
        bounds[BOTTOM] = height;
        fps = 0.0;
        total_frames = 0.0;
        total_seconds = 0.0;
        score = 0;
        stage = 1;
        time_start = clock();
        last_frame_time = time_start;
    }

    void set_game_over() {
        game_over = true;
    }

    void reset_game_over() {
        game_over = false;
    }

    bool is_game_over() {
        return game_over;
    }

    uint get_game_over_threshold() {
        return game_over_threshold;
    }

    uint get_width() {
        return width;
    }

    uint get_height() {
        return height;
    }

    double get_entity_width() {
        return entity_width;
    }

    double get_entity_height() {
        return entity_height;
    }

    uint get_stage() {
        return stage;
    }

    void increase_stage() {
        stage++;
    }

    void add_score(uint score) {
        this->score += score;
    }

    uint get_score() {
        return score;
    }

    double get_fps() {
        if (total_seconds != 0) {
            fps = total_frames / total_seconds;
        }
        return fps;
    }

    double get_delta_time() {
        return delta_time;
    }

    void update_frame() {
        total_frames++;
        total_seconds = (clock() - time_start) / (double)CLOCKS_PER_SEC;
        delta_time = (clock() - last_frame_time) / (double)CLOCKS_PER_SEC;
        last_frame_time = clock();
    }

    double get_bounds(Bounds direction) {
        return bounds[direction];
    }

public:
    uint max_aliens;
    uint max_aliens_per_row;
    uint max_rows;

private:
    uint score;
    uint stage;
    uint width;
    uint height;
    uint game_over_threshold;
    bool game_over;
    double time_start;
    double fps;
    double total_frames;
    double total_seconds;
    double last_frame_time;
    double delta_time;
    double entity_width;
    double entity_height;
    double bounds[DIRECTIONS];
};

Core *core;

#endif