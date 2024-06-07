#pragma once

#include <SDL.h>
#include <chrono>
#include <ctime>
#include <memory>

#define DIRECTIONS 4

typedef unsigned int uint;

using namespace std::chrono;

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
    Core();
    void init(uint width, uint height, uint max_aliens_per_row, uint max_rows);
    void set_game_over();
    void reset_game_over();
    bool is_game_over();
    uint get_game_over_threshold();
    uint get_width();
    uint get_height();
    int get_entity_width();
    int get_entity_height();
    void reset_score();
    void add_score(uint score);
    uint get_score();
    double get_fps();
    double get_delta_time();
    void update_frame();
    double get_bounds(Bounds direction);

public:
    uint max_aliens;
    uint max_aliens_per_row;
    uint max_rows;

private:
    uint score;
    uint width;
    uint height;
    uint game_over_threshold;
    int entity_width = 20;
    int entity_height = 10;
    bool game_over;
    double delta_time;
    double fps = 0.0;
    double total_frames = 0.0;
    double total_seconds = 0.0;
    double last_frame_time = 0.0;
    double bounds[DIRECTIONS];
    time_point<system_clock> time_start;
};

extern Core core;