#pragma once
#include "frame/Frame.h"
#include "frame/Entity.h"
#include "components/BackgroundTexture.h"
#include "components/BoxNode.h"
#include "components/PhysicsBody.h"
#include "components/PhysicsWorld.hpp"
using frame::Frame;
using frame::Entity;

Entity* box_entity(Frame* f, Entity* parent = 0, int slot_x = 0, int slot_y = 0) {

    // Create a root metabox
    auto e = f->add_entity<BoxNode, PhysicsWorld>();

    // Add the background texture
    e->add_component<BackgroundTexture>()
     ->set("7grid.png");


    // If parent is set, add necessary extra parameters
    if (parent) {
        e->get_component<BoxNode>()
         ->set_parent(parent, slot_x, slot_y);
    }

    // Return the new metabox
    return e;
}

Entity* game_entity(Frame* f, Entity* box) {
    auto e = f->add_entity<PhysicsBody>();

    //
    e->add_component<GameEntity>()
     ->set_container(box);

    //
    e->add_component<Texture>()
     ->set("jacob.png");

    //
    return e;
}
