#pragma once

#include <memory>
#include <cstdint>

#include "../entities/entity.hpp"

#define MAX_ENTITIES 256

class EntityList {
public:
    EntityList();

    void clear();
    bool is_full();
    int find(std::shared_ptr<Entity> e);
    int how_many_instances_of(EntityClass c);
    void instantiate(std::shared_ptr<Entity> e);
    void remove(std::shared_ptr<Entity> e);
    std::shared_ptr<Entity> at(int index);
    std::shared_ptr<Entity> operator[](int index);
    int size();

private:
    void remove_entity(std::shared_ptr<Entity> e);

private:
    int entity_counter = 0;
    uint64_t ids = 0;
    std::shared_ptr<Entity> entity_list[MAX_ENTITIES];
};