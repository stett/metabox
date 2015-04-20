
/*

System: BoxTraversal

Handles traversals over the BoxNode tree, and the GameEntityContainer
network. If a GameEntity changes its container or if a BoxNode changes
its parent, parent_changed() will be called in order to move the physics
body of the moved entity to the world of the new parent.

*/

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


class BoxTraversal : public System {

 protected:
    void start() {

        // Hook up event listeners
        GameEntity::container_changed.add_listener(this, &BoxTraversal::entity_container_changed);
        BoxNode::parent_changed.add_listener(this, &BoxTraversal::node_parent_changed);
    }

    void entity_container_changed(GameEntity* game_entity, Entity* e_new_container, Entity* e_old_container) {
        Entity* e = game_entity->get_entity();
        parent_changed(e, e_new_container, e_old_container);
    }

    void node_parent_changed(BoxNode* box_node, BoxNode* new_parent, BoxNode* old_parent) {
        Entity* e = box_node->get_entity();
        Entity* e_new_parent = new_parent ? new_parent->get_entity() : 0;
        Entity* e_old_parent = old_parent ? old_parent->get_entity() : 0;
        parent_changed(e, e_new_parent, e_old_parent);
    }

    void parent_changed(Entity* e, Entity* e_new_container, Entity* e_old_container) {

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
