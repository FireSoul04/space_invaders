#ifndef SPACE_INVADERS_H
#define SPACE_INVADERS_H

#include <SDL2/SDL.h>

#include "core.hpp"
#include "keybuffer.hpp"
#include "entitylist.hpp"
#include "entity.hpp"
#include "ship.hpp"
#include "alien.hpp"
#include "wall.hpp"
#include "swarm.hpp"
#include "projectile.hpp"

#define MAX_WALLS 4

inline void init(EntityList& entities);
inline void loop(SDL_Window *window, SDL_Renderer *renderer, EntityList& entities);

inline void render(SDL_Renderer *renderer, EntityList& entities, uint lives);
inline void render_all(SDL_Renderer *renderer, EntityList& entities);
inline void render_string(SDL_Renderer *renderer, const char *str, int x, int y);

// Adds the pressed key if not in the key buffer
// and removes the released key if in the key buffer
inline bool get_input(KeyBuffer& keybuff);
inline void key_pressed(SDL_Keycode key, KeyBuffer& keybuff);
inline void key_released(SDL_Keycode key, KeyBuffer& keybuff);

inline void check_game_over(Swarm& swarm);

// Updates all entities in the entity list calling the
// update methods of each entity subclass
inline void update(EntityList& entities, Swarm& s);

// Player handler
inline void move_player(KeyBuffer& keybuff, Ship *player);
inline void shoot_player(EntityList& entities, KeyBuffer& keybuff, Ship *player);

// Walls handler
inline void spawn_walls(EntityList& entities);

// Aliens handler
inline void spawn_aliens(EntityList& entities, Swarm& s);
inline void check_aliens_alive(EntityList& entities, Swarm& s);
inline void shoot_swarm(EntityList& entities, Swarm& s);

// Projectiles handler
inline void remove_projectiles(EntityList& entities, Swarm& s, Projectile *p);
inline void remove_entity_colliding(EntityList& entities, Swarm& s, Entity *e);

#endif