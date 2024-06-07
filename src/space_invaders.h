#pragma once

#include <memory>
#include <SDL.h>

#include "game_core/core.hpp"
#include "game_core/keybuffer.hpp"
#include "game_core/entitylist.hpp"
#include "entities/entity.hpp"
#include "entities/ship.hpp"
#include "entities/alien.hpp"
#include "entities/wall.hpp"
#include "entities/swarm.hpp"
#include "entities/projectile.hpp"

constexpr int MAX_WALLS = 4;

void init(EntityList& entities);
void loop(SDL_Window *window, SDL_Renderer *renderer, EntityList& entities);
void menu(SDL_Renderer *renderer, KeyBuffer& keybuff);

void render(SDL_Renderer *renderer, EntityList& entities, uint lives);
void render_all(SDL_Renderer *renderer, EntityList& entities);
void render_string(SDL_Renderer *renderer, const char *str, int x, int y);

// Adds the pressed key if not in the key buffer
// and removes the released key if in the key buffer
bool get_input(KeyBuffer& keybuff);
void key_pressed(SDL_Keycode key, KeyBuffer& keybuff);
void key_released(SDL_Keycode key, KeyBuffer& keybuff);

void check_game_over(EntityList& entities, std::shared_ptr<Swarm>& s);

// Updates all entities in the entity list calling the
// update methods of each entity subclass
void update(EntityList& entities, KeyBuffer& keybuff, std::shared_ptr<Swarm>& swarm, std::shared_ptr<Ship>& player);

// Player handler
void move_player(KeyBuffer& keybuff, std::shared_ptr<Ship>& player);
void shoot_player(EntityList& entities, KeyBuffer& keybuff, std::shared_ptr<Ship>& player);

// Walls handler
void spawn_walls(EntityList& entities);

// Aliens handler
void spawn_aliens(EntityList& entities, std::shared_ptr<Swarm>& s);
void check_aliens_alive(EntityList& entities, std::shared_ptr<Swarm>& s);
void shoot_swarm(EntityList& entities, std::shared_ptr<Swarm>& s);

// Projectiles handler
void remove_projectiles(EntityList& entities, std::shared_ptr<Swarm>& s, std::shared_ptr<Projectile>& p);
void remove_entity_colliding(EntityList& entities, std::shared_ptr<Swarm>& s, std::shared_ptr<Entity>& e);