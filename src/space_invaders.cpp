#include <iostream>
#include <memory>
#include <string>
#include <ctime>

#include <SDL.h>
#include <SDL_ttf.h>

#include "config.h"
#include "space_invaders.h"

int main(int argc, char **argv) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    if (TTF_Init() < 0) {
        std::cerr << TTF_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    srand(time(nullptr));
    core.init(800, 600, 10, 5);

    window = SDL_CreateWindow("Space invaders",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        core.get_width(),
        core.get_height(),
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::exit(EXIT_FAILURE);
    }

    EntityList entities;    // Entity handler
    init(entities);
    loop(window, renderer, entities);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void init(EntityList& entities) {
    // Player details
    double player_init_x = core.get_width() / 2;
    double player_init_y = core.get_height() * 7 / 8;
    double player_speed = 200.0;
    double player_shoot_frequency = 1.0; // Seconds
    std::shared_ptr<SDL_Rect> rect = std::make_shared<SDL_Rect>();
    rect->w = core.get_entity_width();
    rect->h = core.get_entity_height();
    std::shared_ptr<Ship> player = std::make_shared<Ship>(player_init_x, player_init_y, player_speed, player_shoot_frequency, rect, green);

    // Swarm details
    double swarm_init_x = core.get_bounds(LEFT);
    double swarm_init_y = core.get_bounds(TOP);
    double swarm_speed = 20.0;
    double swarm_shoot_frequency = 1.0;
    std::shared_ptr<Swarm> swarm = std::make_shared<Swarm>(swarm_init_x, swarm_init_y, swarm_speed, swarm_shoot_frequency);

    entities.instantiate(player);
    entities.instantiate(swarm);
    spawn_aliens(entities, swarm);
    spawn_walls(entities);
}

void loop(SDL_Window *window, SDL_Renderer *renderer, EntityList& entities) {
    bool running;           // Condition to break the main game loop
    KeyBuffer keybuff;      // Buffer for keyboard key presses

    std::shared_ptr<Ship> player = std::static_pointer_cast<Ship>(entities[0]);    // First entity is always player
    std::shared_ptr<Swarm> swarm = std::static_pointer_cast<Swarm>(entities[1]);

    running = true;
    while (running) {
        menu(renderer, keybuff);

        core.reset_game_over();
        while (running && !core.is_game_over() && 
            !keybuff.find_key(SDLK_ESCAPE)) {

            running = get_input(keybuff);

            update(entities, keybuff, swarm, player);

            render(renderer, entities, player->get_lives());
            core.update_frame();
            std::cout << "FPS: " << core.get_fps() << "\033[1A" << std::endl;
        }
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
        SDL_RenderClear(renderer);
        render_string(renderer, "Game over", core.get_width() / 2 - 60, core.get_height() / 2);
        SDL_RenderPresent(renderer);
        entities.clear();

        while (running && !keybuff.find_key(SDLK_ESCAPE)) {
            running = get_input(keybuff);
        }
    }
}

void menu(SDL_Renderer *renderer, KeyBuffer& keybuff) {
    uint title_pos_x = core.get_width() / 2 - 80;
    uint title_pos_y = core.get_height() / 5;

    uint start_pos_x = core.get_width() / 2 - 40;
    uint start_pos_y = core.get_height() * 5 / 8;
    uint settings_pos_y = core.get_height() * 6 / 8;
    uint ready_pos_y = core.get_height() / 2;

    uint arrow_pos_x = start_pos_x - 20;
    uint arrow_pos_y = start_pos_y;
    
    bool running = true;
    do {
        get_input(keybuff);
        while (!keybuff.find_key(SDLK_SPACE) && !keybuff.find_key(SDLK_RETURN)) {
            running = get_input(keybuff);
            if (keybuff.find_key(SDLK_w) && arrow_pos_y == settings_pos_y) {
                arrow_pos_y = start_pos_y;
            }
            if (keybuff.find_key(SDLK_s) && arrow_pos_y == start_pos_y) {
                arrow_pos_y = settings_pos_y;
            }

            SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
            SDL_RenderClear(renderer);
            render_string(renderer, "Space Invaders", title_pos_x, title_pos_y);
            render_string(renderer, "Start", start_pos_x, start_pos_y);
            render_string(renderer, "Settings", start_pos_x, settings_pos_y);
            render_string(renderer, ">", arrow_pos_x, arrow_pos_y);
            SDL_RenderPresent(renderer);

            if (!running || keybuff.find_key(SDLK_ESCAPE)) {
                std::exit(EXIT_SUCCESS);
            }
        }

        get_input(keybuff);
        while (!keybuff.find_key(SDLK_SPACE) && !keybuff.find_key(SDLK_RETURN)) {
            running = get_input(keybuff);

            SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
            SDL_RenderClear(renderer);
            render_string(renderer, "Space Invaders", title_pos_x, title_pos_y);
            render_string(renderer, "Press enter or space to go back", title_pos_x - 80, settings_pos_y);
            SDL_RenderPresent(renderer);
            
            if (!running || keybuff.find_key(SDLK_ESCAPE)) {
                std::exit(EXIT_SUCCESS);
            }
        }
    } while (arrow_pos_y != start_pos_y);

    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderClear(renderer);
    render_string(renderer, "Ready?", start_pos_x, ready_pos_y);
    SDL_RenderPresent(renderer);

    int time = 0;
    while (time <= 1000) {
        if (!get_input(keybuff) || keybuff.find_key(SDLK_ESCAPE)) {
            std::exit(EXIT_SUCCESS);
        }
        time += 250;
        _sleep(250);
    }
}

void update(EntityList& entities, KeyBuffer& keybuff, std::shared_ptr<Swarm>& swarm, std::shared_ptr<Ship>& player) {
    move_player(keybuff, player);
    shoot_player(entities, keybuff, player);

    check_game_over(entities, swarm);
    check_aliens_alive(entities, swarm);
    shoot_swarm(entities, swarm);

    for (int i = 0; i < entities.size(); i++) {
        std::shared_ptr<Entity> e = entities[i];

        e->update();
        if (e is PROJECTILE) {
            std::shared_ptr<Projectile> p = std::static_pointer_cast<Projectile>(e);
            remove_projectiles(entities, swarm, p);
        }
    }
}

void render(SDL_Renderer *renderer, EntityList& entities, uint lives) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_all(renderer, entities);
    std::string score = "Score: " + std::to_string(core.get_score());
    render_string(renderer, score.c_str(), 20, 20);
    std::string lives_s = "Lives: " + std::to_string(lives);
    render_string(renderer, lives_s.c_str(), core.get_width() - 100, 20);

    SDL_RenderPresent(renderer);
}

void render_all(SDL_Renderer *renderer, EntityList& entities) {
    for (int i = 0; i < entities.size(); i++) {
        std::shared_ptr<Entity> e = entities[i];
        SDL_Rect *rect = e->get_sprite().get();
        SDL_Color color = e->get_color();

        if (e->is_alive()) {
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, rect);
        } else {
            e->animate_death(renderer);
            entities.remove(e);
        }
    }
}

void render_string(SDL_Renderer *renderer, const char *str, int x, int y) {
    TTF_Font *font = TTF_OpenFont(FONT_PATH, 24);
    if (font == nullptr) {
        std::cerr << "Font failed to create: " << TTF_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, str, white);
    if (textSurface == nullptr) {
        std::cerr << TTF_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    SDL_FreeSurface(textSurface);

    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
    SDL_Rect renderQuad = { x, y, textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
    SDL_DestroyTexture(textTexture);
}

void check_game_over(EntityList& entities, std::shared_ptr<Swarm>& s) {
    double last_y = s->last_row_alien_height();
    if (last_y >= core.get_game_over_threshold()) {
        core.set_game_over();
    }
}

void spawn_walls(EntityList& entities) {
    std::shared_ptr<Wall> w;
    std::shared_ptr<SDL_Rect> rect;
    double init_y = 420; // The walls spawn at the same y
    double init_x = core.get_bounds(LEFT) + 100;
    double offset_x = core.get_entity_width() * 8; // Distance from the wall at his left

    for (int i = 0; i < MAX_WALLS; i++) {
        rect = std::make_shared<SDL_Rect>();
        rect->w = core.get_entity_width() * 2;
        rect->h = core.get_entity_width() * 2;
        w = std::make_shared<Wall>(init_x, init_y, rect, green);
        entities.instantiate(w);
        init_x += offset_x; 
    }
}

void spawn_aliens(EntityList& entities, std::shared_ptr<Swarm>& s) {
    std::shared_ptr<Alien> a;
    std::shared_ptr<SDL_Rect> rect;
    AlienType type;
    double init_x;
    double init_y;
    double offset_x;
    double offset_y;

    s->restart();
    for (unsigned int i = 0; i < core.max_aliens; i++) {
        rect = std::make_shared<SDL_Rect>();
        rect->w = core.get_entity_width();
        rect->h = core.get_entity_height();
        offset_x = core.get_entity_width() * 3 * (i % core.max_aliens_per_row);
        offset_y = 40 * (i / core.max_aliens_per_row);
        init_x = core.get_bounds(LEFT) + offset_x;
        init_y = 100 + offset_y;
        type = (AlienType)(((core.max_aliens - i) / (core.max_aliens_per_row * 2)) + 1);
        a = std::make_shared<Alien>(init_x, init_y, type, rect, white);
        s->add_alien(a, i);
        entities.instantiate(a);
    }
}

void check_aliens_alive(EntityList& entities, std::shared_ptr<Swarm>& s) {
    static uint next_stage_threshold = core.max_aliens / 2;

    if (entities.how_many_instances_of(ALIEN) == 0) {
        next_stage_threshold = core.max_aliens / 2;
        spawn_aliens(entities, s);
    } else if (entities.how_many_instances_of(ALIEN) <= (int)next_stage_threshold) {
        next_stage_threshold /= 2;
        s->double_stats();
    }
}

void shoot_swarm(EntityList& entities, std::shared_ptr<Swarm>& s) {
    std::shared_ptr<Projectile> p = s->shoot();
    if (p != nullptr) {
        entities.instantiate(p);
    }
}

void remove_projectiles(EntityList& entities, std::shared_ptr<Swarm>& s, std::shared_ptr<Projectile>& p) {
    bool is_colliding = false;
    std::shared_ptr<Entity> entity_hit = nullptr;
    for (int i = 0; i < entities.size() && !is_colliding; i++) {
        is_colliding = p->is_colliding(entities[i]);
        if (is_colliding) {
            entity_hit = entities[i];
        }
    }

    if (is_colliding) {
        remove_entity_colliding(entities, s, entity_hit);
        entities.remove(p);
    } else if (p->out_of_bounds()) {
        entities.remove(p);
    }
}

void remove_entity_colliding(EntityList& entities, std::shared_ptr<Swarm>& s, std::shared_ptr<Entity>& e) {
    if (e is ALIEN) {
        s->remove_alien(std::static_pointer_cast<Alien>(e));
    }
    e->die();
}

// Movement handler
void move_player(KeyBuffer& keybuff, std::shared_ptr<Ship>& player) {
    double velocity = 0.0;
    double speed = 1.0;
    
    if (keybuff.find_key(SDLK_a)) {
        velocity -= speed;
    }
    if (keybuff.find_key(SDLK_d)) {
        velocity += speed;
    }
    player->set_velocity(velocity);
}

// Shooting handler
void shoot_player(EntityList& entities, KeyBuffer& keybuff, std::shared_ptr<Ship>& player) {
    if (keybuff.find_key(SDLK_SPACE)) {
        std::shared_ptr<Projectile> p = player->shoot();
        if (p != nullptr) {
            entities.instantiate(p);
        }
    }
}

bool get_input(KeyBuffer& keybuff) {
    SDL_Event event;

    // Event handler
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
            break;
        case SDL_KEYDOWN:
            key_pressed(event.key.keysym.sym, keybuff);
            break;
        case SDL_KEYUP:
            key_released(event.key.keysym.sym, keybuff);
            break;
        }
    }
    return true;
}

void key_pressed(SDL_Keycode key, KeyBuffer& keybuff) {
    switch (key) {
    case SDLK_w:
    case SDLK_s:
    case SDLK_a:
    case SDLK_d:
    case SDLK_SPACE:
    case SDLK_ESCAPE:
    case SDLK_RETURN:
        if (!keybuff.find_key(key)) {
            keybuff.push(key);
        }
        break;
    }
}

void key_released(SDL_Keycode key, KeyBuffer& keybuff) {
    switch (key) {
    case SDLK_w:
    case SDLK_s:
    case SDLK_a:
    case SDLK_d:
    case SDLK_SPACE:
    case SDLK_ESCAPE:
    case SDLK_RETURN:
        keybuff.remove_key(key);
        break;
    }
}