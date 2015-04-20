#pragma once

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "frame/Frame.h"
#include "frame/System.h"
#include "frame/Node.h"
//#include "components/Sprite.hpp"
using std::shared_ptr;
using std::string;
using frame::Frame;
using frame::System;
using frame::Node;


class Window : public System {
 private:
    struct View {
        float x, tx;
        float y, ty;
        float scale, tscale;
        float angle, tangle;
        View() : x(0), tx(0), y(0), ty(0), scale(1), tscale(1), angle(0), tangle(0) {}
    };

 private:
    shared_ptr<sf::RenderWindow> window;
    View view;
    Node* sprites;

 public:
    Window() : sprites(0) {}

 protected:
    void get_view_transforms(sf::RenderStates& states) {
        states.transform
              .translate(sf::Vector2f(
                window->getSize().x * .5f,
                window->getSize().y * .5f))
              .scale(sf::Vector2f(view.scale, view.scale))
              .translate(sf::Vector2f(view.x, view.y))
              .translate(-sf::Vector2f(
                window->getSize().x * .5f,
                window->getSize().y * .5f));
    }

    void start() {

        // Collect entities with sprite components
        //sprites = f->add_node<Sprite>();

        // Create the window object
        window = shared_ptr<sf::RenderWindow>(
            new sf::RenderWindow(
                sf::VideoMode(800, 600),
                "Frame Sandbox",
                sf::Style::Close | sf::Style::Titlebar));
    }

    void step_begin() {
        window->clear(sf::Color(100, 100, 100));
    }

    void step() {
        //for (auto e : *sprites)
        //    window->draw(e->get_component<Sprite>()->sprite);
    }

    void step_end() {
        window->display();
    }

    void stop() {
        window->close();
        window.reset();
    }
};
