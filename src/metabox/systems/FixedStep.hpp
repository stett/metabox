#pragma once

#include <SFML/System/Clock.hpp>
#include "frame/System.h"

using frame::System;

class FixedStep : public System {
    float dt_min;
    float dt_accum;
    sf::Time t0;
    sf::Time t1;
    sf::Clock clock;

 public:
    explicit FixedStep(float dt_min_ = 1.f/30.f) :
        dt_min(dt_min_), dt_accum(0) {}

 protected:
    void step() {
        t1 = clock.getElapsedTime();
        float dt = (t1 - t0).asSeconds();
        t0 = t1;
        dt_accum += dt;
        while (dt_accum >= dt_min) {
            update(dt_min);
            dt_accum -= dt_min;
        }
    }

    virtual void update(float dt) = 0;
};
