#pragma once
#include <Box2D/Box2D.h>
#include "frame/System.h"
#include "frame/Entity.h"
#include "components/BoxNode.h"
#include "components/PhysicsBody.h"
#include "components/PhysicsWorld.hpp"
using frame::System;
using frame::Node;
using frame::Entity;


class BoxPhysics : public System {

 protected:
    void start() {

        // Add event listeners
        BoxNode::door_opened.add_listener(this, &BoxPhysics::door_opened);
        BoxNode::door_closed.add_listener(this, &BoxPhysics::door_closed);
    }

    void generate_box_edges(Entity* e) {

    }

    void generate_world_edges(Entity* e) {

    }


    // Event listeners
 public:
    void door_opened(BoxDoor* door) {
        generate_box_edges(door->slot->node->get_entity());
    }

    void door_closed(BoxDoor* door) {
        generate_box_edges(door->slot->node->get_entity());
    }

};
