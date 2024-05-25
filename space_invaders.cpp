#include <iostream>
#include <ctime>
#include <SDL.h>

#include "core.hpp"
#include "keybuffer.hpp"
#include "entitylist.hpp"
#include "entity.hpp"
#include "ship.hpp"
#include "alien.hpp"
#include "swarm.hpp"
#include "projectile.hpp"
#include "space_invaders.h"

void test_swarm(EntityList& entities, Swarm *s) {
    
}

int main(int argc, char **argv) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }
    
    srand(time(NULL));
    core = new Core(800, 600, 10, 5);

    window = SDL_CreateWindow("Space invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, core->get_width(), core->get_height(), SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
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
    EntityList entities = EntityList();
    // Buffer for keyboard key presses
    KeyBuffer keybuff = KeyBuffer();

    // Player statistics
    double player_init_x = core->get_width() / 2;
    double player_init_y = core->get_height() * 7 / 8;
    double player_speed = 200.0;
    double player_shoot_frequency = 1.0;
    SDL_Rect *rect = new SDL_Rect();
    rect->w = 10;
    rect->h = 10;
    Ship *player = new Ship(player_init_x, player_init_y, player_speed, player_shoot_frequency, rect, green);
    Swarm *swarm = spawn_aliens(entities);
    
    running = true;

    entities.instantiate(player);
    //do {
        core->reset_game_over();
        while (running && !core->is_game_over()) {
            running = get_input(keybuff);
            move_player(keybuff, player);
            //shoot_player(entities, keybuff, player);
            if (keybuff.find_key(SDLK_SPACE)) {
                test_swarm(entities);
            }

            if (entities.how_many_instances_of(ALIEN) == 0) {
                delete swarm;   // Removes the last swarm of aliens to create the next one
                swarm = spawn_aliens(entities);
            }
            shoot_swarm(entities, swarm);

            update(entities, swarm);

            render(renderer, entities);
            core->update_frame();
            std::cout << "Score: " << core->get_score() << std::endl << "\033[1A";
        }
        std::cout << "game over";
        entities.clear();
    //} while (cont);
}

Swarm *spawn_aliens(EntityList& entities) {
    Alien *a;
    SDL_Rect *rect;
    double init_x;
    double init_y;
    double offset_x;
    double offset_y;
    double swarm_init_x = core->get_bounds(LEFT);
    double swarm_init_y = core->get_bounds(TOP);
    double swarm_speed = 50.0;
    Swarm *s = new Swarm(swarm_init_x, swarm_init_y, swarm_speed);

    for (int i = 0; i < core->max_aliens; i++) {
        rect = new SDL_Rect();
        offset_x = core->get_entity_width() * 4 * (i % core->max_aliens_per_row);
        offset_y = 40 * (i / core->max_aliens_per_row);
        init_x = core->get_bounds(LEFT) + offset_x;
        init_y = 200 + offset_y;
        rect->w = core->get_entity_width();
        rect->h = core->get_entity_width();
        a = new Alien(init_x, init_y, s->get_speed(), rect, white);
        s->add_alien(a, i);
        entities.instantiate(a);
    }
    return s;
}

void update(EntityList& entities, Swarm *s) {
    s->update();

    for (int i = 0; i < entities.size(); i++) {
        Entity *e = entities[i];
        e->update();
        if (e->get_class() == PROJECTILE) {
            remove_projectiles(entities, s, (Projectile *)e);
        }
    }
}

void remove_projectiles(EntityList& entities, Swarm *s, Projectile *p) {
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
            s->remove_alien((Alien *)entity_hit);
        }
        entities.remove(p);
        entities.remove(entity_hit);
    } else if (p->out_of_bounds()) {
        entities.remove(p);
    }
}

void shoot_swarm(EntityList& entities, Swarm *s) {
    Projectile *p = s->shoot();
    if (p != nullptr) {
        entities.instantiate(p);
    }
}

void render(SDL_Renderer *renderer, EntityList& entities) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_all(renderer, entities);

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

void key_pressed(SDL_Keycode key, KeyBuffer& keybuff) {
    switch (key) {
    case SDLK_a:
    case SDLK_d:
    case SDLK_SPACE:
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
        keybuff.remove_key(key);
        break;
    }
}