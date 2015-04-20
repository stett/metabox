#include "frame/Entity.h"
#include "components/GameEntity.h"
#include "components/GameEntityContainer.h"
using frame::Entity;

REGISTER_FRAME_COMPONENT(GameEntity);

Event<GameEntity*, Entity*, Entity*> GameEntity::container_changed;

GameEntity* GameEntity::set_container(Entity* e_container) {

    // If we had a previous parent, remove ourself
    // from its list of children.
    Entity* e_old_container = container;
    if (container) clear_container();

    // Set the parent pointer
    container = e_container;

    // Add a children component to the new parent if necessary,
    // and add this entity to its child list.
    auto game_entity_container = container->get_or_add_component<GameEntityContainer>();
    game_entity_container->game_entity_list.insert(entity);

    // Trigger the container change signal
    container_changed.trigger(this, e_container, e_old_container);

    // Return this parent for command chaining
    return this;
}

GameEntity* GameEntity::clear_container() {

    // If parent was unset, then we don't need to remove this
    // from its children.
    if (container == 0)
        return this;

    // Remove ourselves from our old parent's child list
    auto game_entity_container = container->get_component<GameEntityContainer>();
    game_entity_container->game_entity_list.erase(entity);

    // Return this parent for command chaining
    return this;
}
