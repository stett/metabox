#pragma once
#include "frame/Frame.h"
#include "frame/Entity.h"
#include "components/BackgroundTexture.h"
#include "components/BoxNode.h"
#include "components/PhysicsBody.h"
#include "components/PhysicsWorld.hpp"
#include "settings.h"
using frame::Frame;
using frame::Entity;

Entity* box_entity(Frame* f, Entity* parent = 0, int slot_x = 0, int slot_y = 0) {

    // Create a root metabox
    auto e = f->add_entity<BoxNode>();

    // Add the background texture
    e->add_component<BackgroundTexture>()
     ->set("7grid.png");


    // Add the physics world
    e->add_component<PhysicsWorld>()->get_world()->SetGravity(b2Vec2(0, GRAVITY));

    // Add the physics body definitions
    auto physics_body = e->add_component<PhysicsBody>();
    float size = (float)BOX_PHYSICAL_SIZE / 7.f;

    // Create the new box's physics body definitions
    physics_body->body_def.type = b2BodyType::b2_kinematicBody;
    physics_body->body_def.position = b2Vec2((slot_x + .5f) * size, (slot_y + .5f) * size);
    physics_body->body_def.fixedRotation = true;

    // Set the box hull shape
    physics_body->shape.SetAsBox(.5f * size, .5f * size);

    // Set the hull fixture
    physics_body->fixture_def.shape = &physics_body->shape;
    physics_body->fixture_def.friction = FRICTION;
    physics_body->fixture_def.filter.categoryBits = B2_CAT_BOX_HULL;
    physics_body->fixture_def.filter.maskBits = B2_CAT_MAIN | B2_CAT_BOX_HULL;

    // If parent is set, use it as the parent entity for the box-node component
    if (parent) {
        e->get_component<BoxNode>()
         ->set_parent(parent, slot_x, slot_y);
    }

    // Return the new metabox
    return e;
}

Entity* game_entity(Frame* f, Entity* container) {
    auto e = f->add_entity<>();

    // Set the physics body
    auto physics_body = e->add_component<PhysicsBody>();

    // Make a new body for the player in the new world
    float size = (float)BOX_PHYSICAL_SIZE / 7.f;
    physics_body->body_def.type = b2BodyType::b2_dynamicBody;
    //physics_body->body_def.position = position;
    physics_body->body_def.fixedRotation = true;

    // Set the box hull shape
    physics_body->shape.SetAsBox(.3f * size, .3f * size);

    // Create a rectangular fixture for him
    physics_body->fixture_def.shape = &physics_body->shape;
    physics_body->fixture_def.friction = FRICTION;
    physics_body->fixture_def.filter.categoryBits = B2_CAT_MAIN;
    physics_body->fixture_def.filter.maskBits = B2_CAT_MAIN;

    //
    e->add_component<GameEntity>()
     ->set_container(container);

    //
    e->add_component<Texture>()
     ->set("jacob.png");

    //
    return e;
}
