#ifndef SPACE_INVADERS_H
#define SPACE_INVADERS_H

#include <SDL2/SDL.h>

#include "core.hpp"
#include "keybuffer.hpp"
#include "entitylist.hpp"
#include "entity.hpp"
#include "ship.hpp"
#include "alien.hpp"
#include "swarm.hpp"
#include "projectile.hpp"

inline void loop(SDL_Window *window, SDL_Renderer *renderer);
inline void render(SDL_Renderer *renderer, EntityList& entities);
inline void render_all(SDL_Renderer *renderer, EntityList& entities);
inline void render_string(SDL_Renderer *renderer, const char *str);

// Adds the pressed key if not in the key buffer
// and removes the released key if in the key buffer
inline bool get_input(KeyBuffer& keybuff);
inline void key_pressed(SDL_Keycode key, KeyBuffer& keybuff);
inline void key_released(SDL_Keycode key, KeyBuffer& keybuff);

// Updates all entities in the entity list calling the
// update methods of each entity subclass
inline void update(EntityList& entities, Swarm *s);

// Player handler
inline void move_player(KeyBuffer& keybuff, Ship *player);
inline void shoot_player(EntityList& entities, KeyBuffer& keybuff, Ship *player);

// Aliens handler
inline Swarm *spawn_aliens(EntityList& entities);
inline void shoot_swarm(EntityList& entities, Swarm *s);

// Projectiles handler
inline void remove_projectiles(EntityList& entities, Swarm *s, Projectile *p);

#endif