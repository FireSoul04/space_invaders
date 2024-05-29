#include <iostream>
#include <string>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "space_invaders.h"

int main(int argc, char **argv) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0) {
        std::cerr << TTF_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    
    srand(time(nullptr));
    core = new Core(800, 600, 1, 1);

    window = SDL_CreateWindow("Space invaders",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        core->get_width(),
        core->get_height(),
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    loop(window, renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void loop(SDL_Window *window, SDL_Renderer *renderer) {
    bool running;
    //bool cont;

    // Entity handler
    EntityList entities;
    // Buffer for keyboard key presses
    KeyBuffer keybuff;

    // Player statistics
    double player_init_x = core->get_width() / 2;
    double player_init_y = core->get_height() * 7 / 8;
    double player_speed = 200.0;
    double player_shoot_frequency = 0.1;
    SDL_Rect *rect = new SDL_Rect();
    rect->w = 10;
    rect->h = 10;
    Ship *player = new Ship(player_init_x, player_init_y, player_speed, player_shoot_frequency, rect, green);

    double swarm_init_x = core->get_bounds(LEFT);
    double swarm_init_y = core->get_bounds(TOP);
    Swarm swarm = Swarm(swarm_init_x, swarm_init_y);
    spawn_aliens(entities, swarm);
    
    running = true;

    spawn_walls(entities);
    entities.instantiate(player);
    //do {
        core->reset_game_over();
        while (running && !core->is_game_over() && 
            !keybuff.find_key(SDLK_ESCAPE)) {
            running = get_input(keybuff);
            move_player(keybuff, player);
            shoot_player(entities, keybuff, player);

            check_aliens_alive(entities, swarm);
            shoot_swarm(entities, swarm);

            update(entities, swarm);

            render(renderer, entities, player->get_lives());
            core->update_frame();
        }
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
        SDL_RenderClear(renderer);
        render_string(renderer, "Game over", core->get_width() / 2 - 60, core->get_height() / 2);
        SDL_RenderPresent(renderer);
        entities.clear();

        while (running && !keybuff.find_key(SDLK_ESCAPE)) {
            running = get_input(keybuff);
        }
    //} while (cont);
}

void update(EntityList& entities, Swarm& s) {
    s.update();
    for (int i = 0; i < entities.size(); i++) {
        Entity *e = entities[i];
        e->update();
        if (e->get_class() == PROJECTILE) {
            remove_projectiles(entities, s, (Projectile *)e);
        }
    }
}

void render(SDL_Renderer *renderer, EntityList& entities, uint lives) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_all(renderer, entities);
    std::string score = "Score: " + std::to_string(core->get_score());
    render_string(renderer, score.c_str(), 20, 20);
    std::string lives_s = "Lives: " + std::to_string(lives);
    render_string(renderer, lives_s.c_str(), core->get_width() - 100, 20);

    SDL_RenderPresent(renderer);
}

void render_all(SDL_Renderer *renderer, EntityList& entities) {
    for (int i = 0; i < entities.size(); i++) {
        Entity *e = entities[i];
        SDL_Rect *rect = e->get_sprite();
        SDL_Color color = e->get_color();

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, rect);
    }
}

void render_string(SDL_Renderer *renderer, const char *str, int x, int y) {
    TTF_Font *font = TTF_OpenFont("font.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Font failed to create: " << TTF_GetError() << std::endl;
        std::abort();
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, str, white);
    if (textSurface == nullptr) {
        std::cerr << TTF_GetError() << std::endl;
        std::abort();
    }
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        std::cerr << SDL_GetError() << std::endl;
        std::abort();
    }
    SDL_FreeSurface(textSurface);

    // Ottieni le dimensioni della texture
    int textWidth = 0;
    int textHeight = 0;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
    SDL_Rect renderQuad = { x, y, textWidth, textHeight };

    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
    SDL_DestroyTexture(textTexture);
}

void spawn_walls(EntityList& entities) {
    Wall *w;
    SDL_Rect *rect;
    double init_x;
    double init_y;
    double offset_x;

    for (int i = 0; i < MAX_WALLS; i++) {
        rect = new SDL_Rect();
        offset_x = core->get_entity_width() * 8 * i;
        init_x = core->get_bounds(LEFT) + 100 + offset_x;
        init_y = 420;
        rect->w = core->get_entity_width() * 2;
        rect->h = core->get_entity_width() * 2;
        w = new Wall(init_x, init_y, rect, green);
        entities.instantiate(w);
    }
}

void spawn_aliens(EntityList& entities, Swarm& s) {
    Alien *a;
    SDL_Rect *rect;
    double init_x;
    double init_y;
    double offset_x;
    double offset_y;
    double swarm_speed = 25.0 + core->get_stage() * 5.0;
    double swarm_shoot_freq = 5.0 / core->get_stage();

    s.restart();
    s.set_speed(swarm_speed);
    s.set_shoot_frequency(swarm_shoot_freq);

    for (int i = 0; i < core->max_aliens; i++) {
        rect = new SDL_Rect();
        offset_x = core->get_entity_width() * 3 * (i % core->max_aliens_per_row);
        offset_y = 40 * (i / core->max_aliens_per_row);
        init_x = core->get_bounds(LEFT) + offset_x;
        init_y = 100 + offset_y;
        rect->w = core->get_entity_width();
        rect->h = core->get_entity_height();
        a = new Alien(init_x, init_y, s.get_speed(), rect, white);
        s.add_alien(a, i);
        entities.instantiate(a);
    }
}

void check_aliens_alive(EntityList& entities, Swarm& s) {
    if (entities.how_many_instances_of(ALIEN) == 0) {
        core->increase_stage();
        spawn_aliens(entities, s);
    }
}

void shoot_swarm(EntityList& entities, Swarm& s) {
    Projectile *p = s.shoot();
    if (p != nullptr) {
        entities.instantiate(p);
    }
}

void remove_projectiles(EntityList& entities, Swarm& s, Projectile *p) {
    if (p->is_invincible()) {
        return;
    }

    bool is_colliding = false;
    Entity *entity_hit = nullptr;
    for (int i = 0; i < entities.size() && !is_colliding; i++) {
        is_colliding = p->is_colliding(entities[i]);
        if (is_colliding) {
            entity_hit = entities[i];
        }
    }

    if (is_colliding) {
        if (entity_hit->get_class() == ALIEN) {
            s.remove_alien((Alien *)entity_hit);
        }

        if (entity_hit->get_class() == SHIP &&
            ((Ship *)entity_hit)->is_alive()) {
            ((Ship *)entity_hit)->die();
        } else if (entity_hit->get_class() == WALL &&
            ((Wall *)entity_hit)->is_alive()) {
            ((Wall *)entity_hit)->hit();
        } else {
            entities.remove(entity_hit);
        }
        entities.remove(p);
    } else if (p->out_of_bounds()) {
        entities.remove(p);
    }
}

// Movement handler
void move_player(KeyBuffer& keybuff, Ship *player) {
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
void shoot_player(EntityList& entities, KeyBuffer& keybuff, Ship *player) {
    if (keybuff.find_key(SDLK_SPACE)) {
        Projectile *p = player->shoot();
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
    case SDLK_a:
    case SDLK_d:
    case SDLK_SPACE:
    case SDLK_ESCAPE:
        if (!keybuff.find_key(key)) {
            keybuff.push(key);
        }
        break;
    }
}

void key_released(SDL_Keycode key, KeyBuffer& keybuff) {
    switch (key) {
    case SDLK_a:
    case SDLK_d:
    case SDLK_SPACE:
    case SDLK_ESCAPE:
        keybuff.remove_key(key);
        break;
    }
}