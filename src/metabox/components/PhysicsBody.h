/*

Component: PhysicsBody

This body carries around shape, body, and fixture definitions.
The body itself is not instantiated until "set_world" is called.
The definitions are kept so that the body can be destroyed and
more easily recreated in another world.

*/

#pragma once
#include <Box2D/Box2D.h>
#include "frame/Component.h"

class PhysicsWorld;

class PhysicsBody : public frame::Component {

 public:
    b2FixtureDef fixture_def;
    b2BodyDef body_def;
    b2PolygonShape shape;

 private:
    b2Body* body;

 public:
    //PhysicsBody() : body(0) {}
    PhysicsBody();

 public:
    PhysicsBody* set_world(PhysicsWorld* physics_world);
    PhysicsBody* remove_body();
    b2Body* get_body() { return body; }
};
