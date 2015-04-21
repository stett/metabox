#pragma once
#include <Box2D/Box2D.h>
#include <memory>
#include "frame/Component.h"
#include "frame/Event.h"
using std::shared_ptr;
using frame::Event;

class PhysicsWorld : public frame::Component {
    friend class Physics;
    friend class PhysicsBody;


 protected:
    shared_ptr<b2World> world;

 public:
    b2Body* edges;

 public:
    static Event<PhysicsWorld*> created;

 public:
    PhysicsWorld() : edges(0) {
        world = shared_ptr<b2World>(new b2World(b2Vec2(0, 9.8)));
    }

    ~PhysicsWorld() {
        // TODO: Remove all physics bodies from their entities
    }

 protected:
    void init() { created.trigger(this); }

 public:
    shared_ptr<b2World> get_world() { return world; }
};

