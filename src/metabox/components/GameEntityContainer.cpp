#include <cstdarg>
#include "frame/Entity.h"
#include "components/GameEntityContainer.h"
#include "components/GameEntity.h"
using frame::Entity;


REGISTER_FRAME_COMPONENT(GameEntityContainer);

GameEntityContainer* GameEntityContainer::add(Entity* e_game_entity) {

    // Add entity to child list
    game_entity_list.insert(e_game_entity);

    // Add container component to child entity if necessary
    auto game_entity = e_game_entity->get_or_add_component<GameEntity>();
    game_entity->container = entity;

    // Return this children for command chaining
    return this;
}

GameEntityContainer* GameEntityContainer::remove(Entity* e_game_entity) {

    // Remove the child from the child list.
    // If it wasn't in the list, stop here.
    if (game_entity_list.erase(e_game_entity) == 0)
        return this;

    // Clear the child's parent pointer
    auto game_entity = e_game_entity->get_component<GameEntity>();
    game_entity->container = 0;

    // Return this children for command chaining
    return this;
}

GameEntityContainer* GameEntityContainer::clear() {
    while (size())
        remove(*begin());
    return this;
}
