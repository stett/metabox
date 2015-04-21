/*

Component: PhysicsBody

This body carries around shape, body, and fixture definitions.
The body itself is not instantiated until "set_world" is called.
The definitions are kept so that the body can be destroyed and
more easily recreated in another world.

*/

#pragma once
#include <Box2D/Box2D.h>
#include <list>
#include "frame/Component.h"
#include "frame/Event.h"
using std::list;
using frame::Event;

class PhysicsWorld;

class PhysicsBody : public frame::Component {

 public:
    b2FixtureDef fixture_def;
    b2BodyDef body_def;
    list<b2FixtureDef> secondary_fixture_defs;

 public:
    static Event<PhysicsBody*> created;

 private:
    b2Body* body;

 public:
    PhysicsBody();

 protected:
    void add() { created.trigger(this); }

 public:
    PhysicsBody* set_world(PhysicsWorld* physics_world);
    PhysicsBody* remove_body();
    PhysicsBody* update_fixtures();
    b2Body* get_body() { return body; }
};
