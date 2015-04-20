#pragma once
#include "frame/System.h"
#include "frame/Node.h"
#include "frame/Entity.h"
#include "components/GameEntity.h"
#include "components/PhysicsBody.h"
#include "components/PhysicsWorld.hpp"
#include "components/ViewFollow.h"
using frame::System;
using frame::Node;
using frame::Entity;


class GameEntityBoxTraversal : public System {
 private:

 public:
    GameEntityBoxTraversal() {}

 protected:
    void start() {
        GameEntity::container_changed.add_listener(this, &GameEntityBoxTraversal::container_changed);
    }

    void update() {}

    void container_changed(GameEntity* game_entity, Entity* e_new_container, Entity* e_old_container) {
        Entity* e = game_entity->get_entity();

        // Update the game entity's physics body if it has one,
        // and if it's moving into a container with a physics world.
        auto physics_body = e->get_component<PhysicsBody>();
        auto physics_world = e_new_container->get_component<PhysicsWorld>();
        if (physics_body && physics_world) {
            physics_body->set_world(physics_world);
        }

        // If the game entity also has a view-follow component,
        // remove the view-follow component from the previous
        // container, and add it to the new one.
        if (e->get_component<ViewFollow>()) {
            e_old_container->remove_component<ViewFollow>();
            e_new_container->add_component<ViewFollow>();
        }
    }
};
