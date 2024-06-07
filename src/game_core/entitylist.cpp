#include "entitylist.hpp"

EntityList::EntityList() {
}

void EntityList::clear() {
    for (int i = 0; i < entity_counter; i++) {
        remove(entity_list[i]);
    }
}

bool EntityList::is_full() {
    return entity_counter == MAX_ENTITIES;
}

// Return -1 if not found, else the index where it is found
int EntityList::find(std::shared_ptr<Entity> e) {
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

int EntityList::how_many_instances_of(EntityClass c) {
    int instancies = 0;
    for (int i = 0; i < entity_counter; i++) {
        if (entity_list[i] is c) {
            instancies++;
        }
    }
    return instancies;
}

void EntityList::instantiate(std::shared_ptr<Entity> e) {
    if (is_full()) {
        remove_entity(e);
        return;
    }
    e->set_id(ids++);
    entity_list[entity_counter++] = e;
}

void EntityList::remove(std::shared_ptr<Entity> e) {
    int index = find(e);
    if (index != -1) {
        remove_entity(entity_list[index]);

        for (int i = index; i < entity_counter; i++) {
            entity_list[i] = entity_list[i + 1];
        }
        entity_list[entity_counter--] = nullptr;
    }
}

std::shared_ptr<Entity> EntityList::at(int index) {
    return entity_list[index];
}

std::shared_ptr<Entity> EntityList::operator[](int index) {
    return at(index);
}

int EntityList::size() {
    return entity_counter;
}

void EntityList::remove_entity(std::shared_ptr<Entity> e) {
    e = nullptr;
}