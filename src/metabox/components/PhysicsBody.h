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
};
