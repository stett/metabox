#include <Box2D/Box2D.h>
#include "PhysicsBody.h"
#include "PhysicsWorld.hpp"

PhysicsBody::PhysicsBody() : body(0) {
    shape.SetAsBox(.5f, .5f);
    fixture_def.shape = &shape;
}

PhysicsBody* PhysicsBody::set_world(PhysicsWorld* physics_world) {

    // Remove the old body
    remove_body();

    // If there's no new world component, discontinue
    if (!physics_world) return this;

    // Ensure fixture user data are set properly
    body_def.userData = this;
    fixture_def.userData = this;

    // Get the physics world
    auto world = physics_world->world;

    // Create the new body & fixture from the definitions
    body = world->CreateBody(&body_def);
    body->CreateFixture(&fixture_def);

    // Chain
    return this;
}

PhysicsBody* PhysicsBody::remove_body() {

    // No body? No point!
    if (!body) return this;

    // Remove from the current world
    body->GetWorld()->DestroyBody(body);

    // Clear the ptr
    body = 0;

    // Chain
    return this;
}
