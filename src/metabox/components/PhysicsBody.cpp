#include <Box2D/Box2D.h>
#include "PhysicsBody.h"
#include "PhysicsWorld.hpp"

REGISTER_FRAME_COMPONENT(PhysicsBody);


Event<PhysicsBody*> PhysicsBody::created;


PhysicsBody::PhysicsBody() : body(0) {}

PhysicsBody* PhysicsBody::set_world(PhysicsWorld* physics_world) {

    // Remove the old body
    remove_body();

    // If there's no new world component, discontinue
    if (!physics_world) return this;

    // Ensure fixture user data are set properly
    body_def.userData = this;

    // Get the physics world
    auto world = physics_world->world;

    // Create the new body & fixture from the definitions
    body = world->CreateBody(&body_def);

    // Generate the fixtures
    update_fixtures();

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

PhysicsBody* PhysicsBody::update_fixtures() {
    /*
    If the fixture definitions have changed, call this function to
    destroy the old fixtures and to generate the new ones.
    */

    // Can anybody hear meeeeee?
    if (!body) return this;

    // Create the primary fixture
    body->CreateFixture(&fixture_def);

    // Add secondary fixtures
    for (auto secondary_fixture_def : secondary_fixture_defs)
        body->CreateFixture(&secondary_fixture_def);

    // Chain
    return this;
}
