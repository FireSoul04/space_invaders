#ifndef ENTITYLIST_H
#define ENTITYLIST_H

#include <stdint.h>

#include "entity.hpp"
#include "ship.hpp"
#include "alien.hpp"
#include "projectile.hpp"

#define MAX_ENTITIES 256

class EntityList {
public:
    EntityList() {
        entity_counter = 0;
        ids = 0;
    }

    void clear() {
        for (int i = 0; i < entity_counter; i++) {
            remove(entity_list[i]);
        }
    }

    bool is_full() {
        return entity_counter == MAX_ENTITIES;
    }

    // Return -1 if not found, else the index where it is found
    int find(Entity *e) {
        int index = -1;
        bool found = false;
        for (int i = 0; i < entity_counter && !found; i++) {
            if (entity_list[i]->get_id() == e->get_id()) {
                found = true;
                index = i;
            }
        }
        return index;
    }

    int how_many_instances_of(EntityClass c) {
        int instancies = 0;
        for (int i = 0; i < entity_counter; i++) {
            if (entity_list[i]->get_class() == c) {
                instancies++;
            }
        }
        return instancies;
    }

    void instantiate(Entity *e) {
        if (is_full()) {
            remove_entity(e);
            return;
        }
        e->set_id(ids++);
        entity_list[entity_counter++] = e;
    }

    void remove(Entity *e) {
        int index = find(e);
        if (index != -1) {
            remove_entity(entity_list[index]);

            for (int i = index; i < entity_counter; i++) {
                entity_list[i] = entity_list[i + 1];
            }
            entity_list[entity_counter--] = nullptr;
        }
    }

    Entity *at(int index) {
        return entity_list[index];
    }

    Entity *operator[](int index) {
        return at(index);
    }

    int size() {
        return entity_counter;
    }

private:
    void remove_entity(Entity *e) {
        switch (e->get_class()) {
        case SHIP:
            ((Ship *)e)->~Ship();
            break;
        case ALIEN:
            ((Alien *)e)->~Alien();
            break;
        case PROJECTILE:
            ((Projectile *)e)->~Projectile();
            break;
        }
        delete e;
        e = nullptr;
    }

private:
    int entity_counter;
    uint64_t ids;
    Entity *entity_list[MAX_ENTITIES];
};

#endif