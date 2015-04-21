#pragma once
#include <Box2D/Box2D.h>
#include "frame/System.h"
#include "frame/Entity.h"
#include "components/BoxNode.h"
#include "components/PhysicsBody.h"
#include "components/PhysicsWorld.hpp"
#include "settings.h"
using frame::System;
using frame::Node;
using frame::Entity;


class BoxPhysics : public System {

 protected:
    void start() {

        // Add event listeners
        BoxNode::door_opened.add_listener(this, &BoxPhysics::door_opened);
        BoxNode::door_closed.add_listener(this, &BoxPhysics::door_closed);
        BoxNode::created.add_listener(this, &BoxPhysics::node_created);
        PhysicsBody::created.add_listener(this, &BoxPhysics::body_created);
        PhysicsWorld::created.add_listener(this, &BoxPhysics::world_created);
    }

    void generate_box_edges(Entity* e) {

        // Get the physics body
        auto physics_body = e->get_component<PhysicsBody>();
        if (!physics_body) return;

        // Get the node
        auto box_node = e->get_component<BoxNode>();
        if (!box_node) return;

        // Delete the existing edge fixture
        physics_body->secondary_fixture_defs.clear();

        // Generate new edge fixture
        float half = (.5f * (float)BOX_PHYSICAL_SIZE / 7.f);
        for (int i_face = 0; i_face < 4; i_face++) {
            b2Vec2 a, b;
            b2EdgeShape edge_shape;
            b2FixtureDef edge_fixture_def;
            b2Fixture* edge_fixture;
            BoxDoor* door = box_node->get_door((BoxFace)i_face);
            a.Set((i_face == 0 || i_face == 3 ? -1 : 1) * half, (i_face == 0 || i_face == 1 ? -1 : 1) * half);
            b.Set((i_face == 2 || i_face == 3 ? -1 : 1) * half, (i_face == 0 || i_face == 3 ? -1 : 1) * half);
            if (!(door && door->open)) {
                edge_shape.Set(a, b);
                edge_fixture_def.friction = FRICTION;
                edge_fixture_def.filter.categoryBits = B2_CAT_MAIN;
                edge_fixture_def.filter.maskBits = B2_CAT_MAIN;
                physics_body->secondary_fixture_defs.push_back(edge_fixture_def);
            }
        }

        // Re-instantiate the physics body
        physics_body->update_fixtures();
    }

    void generate_world_edges(Entity* e) {

        // Get the physics world
        auto world = e->get_component<PhysicsWorld>();
        if (!world) return;

        // Get the node
        auto box_node = e->get_component<BoxNode>();
        if (!box_node) return;

        // Delete the existing edges body if there is one
        if (world->edges) {
            world->get_world()->DestroyBody(world->edges);
            world->edges = 0;
        }

        // Make the new edges body
        b2BodyDef body_def;
        body_def.type = b2BodyType::b2_staticBody;
        world->edges = world->get_world()->CreateBody(&body_def);
        b2Filter filter;
        filter.categoryBits = B2_CAT_MAIN;
        filter.maskBits = B2_CAT_MAIN | B2_CAT_BOX_HULL;
        float size = BOX_PHYSICAL_SIZE;

        // Create fixtures for each face
        for (int i_face = 0; i_face < 4; i_face++) {
            b2Vec2 a, b;
            b2EdgeShape edge_shape;
            b2Fixture* edge_fixture;
            BoxDoor* door = box_node->get_door((BoxFace)i_face);
            a.Set((i_face == 0 || i_face == 3 ? 0 : size), (i_face == 0 || i_face == 1 ? 0 : size));
            b.Set((i_face == 2 || i_face == 3 ? 0 : size), (i_face == 0 || i_face == 3 ? 0 : size));
            if (door && door->open) {
                b2Vec2 a0, b0;
                if (i_face == Top) {
                    a0.Set(door->slot->x, 0);
                    b0.Set(door->slot->x + 1, 0);
                } else if (i_face == Right) {
                    a0.Set(size, door->slot->y);
                    b0.Set(size, door->slot->y + 1);
                } else if (i_face == Bottom) {
                    a0.Set(door->slot->x + 1, size);
                    b0.Set(door->slot->x, size);
                } else if (i_face == Left) {
                    a0.Set(0, door->slot->y + 1);
                    b0.Set(0, door->slot->y);
                }
                
                edge_shape.Set(a, a0);
                edge_fixture = world->edges->CreateFixture(&edge_shape, 0);
                edge_fixture->SetFriction(FRICTION);
                edge_fixture->SetFilterData(filter);

                edge_shape.Set(b0, b);
                edge_fixture = world->edges->CreateFixture(&edge_shape, 0);
                edge_fixture->SetFriction(FRICTION);
                edge_fixture->SetFilterData(filter);
            } else {
                edge_shape.Set(a, b);
                edge_fixture = world->edges->CreateFixture(&edge_shape, 0);
                edge_fixture->SetFriction(FRICTION);
                edge_fixture->SetFilterData(filter);
            }
        }
    }


    // Event listeners
 public:
    void door_opened(BoxDoor* door) {
        auto e = door->slot->node->get_entity();
        generate_box_edges(e);
        generate_world_edges(e);
    }

    void door_closed(BoxDoor* door) {
        auto e = door->slot->node->get_entity();
        generate_box_edges(e);
        generate_world_edges(e);
    }

    void node_created(BoxNode* node) {
        auto e = node->get_entity();
        if (e->get_component<PhysicsBody>())
            generate_box_edges(e);
        if (e->get_component<PhysicsWorld>())
            generate_world_edges(e);
    }

    void body_created(PhysicsBody* body) {
        auto e = body->get_entity();
        if (e->get_component<BoxNode>())
            generate_box_edges(e);
    }

    void world_created(PhysicsWorld* world) {
        auto e = world->get_entity();
        if (e->get_component<PhysicsWorld>())
            generate_world_edges(e);
    }
};
