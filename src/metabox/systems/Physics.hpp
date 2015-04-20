#pragma once
#include "frame/Node.h"
#include "systems/FixedStep.hpp"
#include "components/PhysicsWorld.hpp"
#include "components/PhysicsBody.h"
using frame::Node;


class Physics : public FixedStep {
 private:
    Node* worlds;
    Node* bodies;

 public:
    Physics() : worlds(0), bodies(0) {}

 protected:
    void start() {
        worlds = f->add_node<PhysicsWorld>();
        bodies = f->add_node<PhysicsBody>();
    }

    void update(float dt) {

        // Step all worlds
        for (auto e : *worlds) {
            e->get_component<PhysicsWorld>()->world->Step(dt, 6, 2);
        }
    }

    void step_end() {

        // Clear old forces from all physics worlds
        for (auto e : *worlds)
            e->get_component<PhysicsWorld>()->world->ClearForces();
    }
};
