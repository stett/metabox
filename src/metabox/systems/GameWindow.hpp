#pragma once

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "frame/Frame.h"
#include "frame/System.h"
#include "frame/Node.h"
#include "components/Texture.h"
#include "components/BackgroundTexture.h"
#include "components/ViewFollow.h"
#include "components/BoxNode.h"
#include "components/GameEntityContainer.h"
#include "components/GameEntity.h"
#include "settings.h"
using std::shared_ptr;
using std::string;
using frame::Frame;
using frame::System;
using frame::Node;


class GameWindow : public System {
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
    Node* follow_box;

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

        // Get the view-follow box
        follow_box = f->add_node<BoxNode, ViewFollow>();

        // Create the window object
        window = shared_ptr<sf::RenderWindow>(
            new sf::RenderWindow(
                sf::VideoMode(800, 600),
                "MetaBox",
                sf::Style::Close | sf::Style::Titlebar));
    }

    void step_begin() {
        window->clear(sf::Color(0, 0, 0));
    }

    void step() {

        // If we have a box to follow, draw it. Just take the first one.
        auto e = *follow_box->begin();
        if (!e) return;

        // Draw the main box
        draw_box(e, sf::Transform());
    }

    void step_end() {
        window->display();
    }

    void stop() {
        window->close();
        window.reset();
    }

    void draw_box(Entity* e, const sf::Transform& t, int depth = 0) {

        // Don't do it if it's too deep
        if (depth >= BOX_RENDER_DEPTH) return;

        // Get this box's node
        auto box_node = e->get_component<BoxNode>();

        // Render the background if there is one
        auto box_bg_texture = e->get_component<BackgroundTexture>();
        if (box_bg_texture) {

            // Scale the background sprite so that it's the correct render size
            const sf::Texture& texture = box_bg_texture->get();
            sf::Sprite sprite; sprite.setTexture(texture);
            sf::Vector2u size = texture.getSize();
            sprite.setScale(sf::Vector2f(
                (float)BOX_RENDER_SIZE / (float)size.x,
                (float)BOX_RENDER_SIZE / (float)size.y));

            // Queue it up.
            window->draw(sprite, sf::RenderStates(t));
        }

        // Render child game entities if there are any
        auto box_entity_container = e->get_component<GameEntityContainer>();
        if (box_entity_container) {

            // Draw each of the game entities
            for (auto game_entity : *box_entity_container) {

                // If the entity has a sprite and a physics body, render it with the box's transforms
                auto game_entity_texture = game_entity->get_component<Texture>();
                auto physics_body = game_entity->get_component<PhysicsBody>();
                if (game_entity_texture && physics_body) {

                    // Get the position
                    auto body = physics_body->get_body();
                    auto pos = body->GetPosition();
                    auto ang = body->GetAngle();
                    auto t_body = sf::Transform()
                        .translate(sf::Vector2f(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER))
                        .rotate(ang * 180.f / 3.14159f);

                    // Draw the sprite
                    const sf::Texture& texture = game_entity_texture->get();
                    auto size = texture.getSize();
                    sf::Sprite sprite;
                    sprite.setTexture(texture);
                    sprite.setOrigin(size.x * .5f, size.y * .5f);
                    window->draw(sprite, sf::RenderStates(t * t_body));
                }
            }
        }

        // Render it's children
        for (auto child : box_node->get_children()) {

            // Get the box node & slot for this child
            auto child_box_node = child->get_component<BoxNode>();
            auto child_slot = child_box_node->get_slot();

            // Generate the child's transform
            auto child_t = sf::Transform()
                .translate(child_slot->x * BOX_RENDER_SIZE / 7.f,
                           child_slot->y * BOX_RENDER_SIZE / 7.f)
                .scale(sf::Vector2f(1.f / 7.f, 1.f / 7.f))
                .rotate(0.f);

            // Draw this child
            draw_box(child, t * child_t, depth + 1);
        }

        // Render outlines around the physics bodies
        auto physics_world = e->get_component<PhysicsWorld>();
        if (physics_world) {
            auto world = physics_world->get_world();

            // Loop through and draw every fixture
            for (b2Body* body = world->GetBodyList(); body; body = body->GetNext()) {

                // Get the graphical transform for the body
                auto pos = body->GetPosition();
                auto ang = body->GetAngle();
                auto t_body = sf::Transform()
                    .translate(sf::Vector2f(pos.x * PIXELS_PER_METER, pos.y * PIXELS_PER_METER))
                    .rotate(ang * 180.f / 3.14159f);

                // Render each fixture in the body
                for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
                    b2Shape* shape = fixture->GetShape();
                    b2Shape::Type type = shape->GetType();

                    // If it's a circle, just use the built in SFML circle shape stuff
                    if (type == b2Shape::Type::e_circle) {
                        b2CircleShape* circle = (b2CircleShape*)shape;
                        sf::CircleShape shape(circle->m_radius);
                        continue;
                    }

                    // If it's not a circle, build up a list of vertices to render based on what it is.
                    sf::VertexArray verts;
                    verts.setPrimitiveType(sf::PrimitiveType::LinesStrip);

                    // Polygon
                    if (fixture->GetType() == b2Shape::Type::e_polygon) {
                        auto shape = (b2PolygonShape*)fixture->GetShape();
                        int num_verts = shape->GetVertexCount();
                        for (int i_vert = 0; i_vert < num_verts; i_vert++) {
                            auto b2_vert = shape->GetVertex(i_vert);
                            sf::Vertex vert(sf::Vector2f(b2_vert.x * PIXELS_PER_METER, b2_vert.y * PIXELS_PER_METER));
                            vert.color = sf::Color(255, 255, 255, 150);
                            verts.append(vert);
                        }
                        verts.append(verts[0]);

                    // Chain
                    } else if (fixture->GetType() == b2Shape::Type::e_chain) {
                        auto shape = (b2ChainShape*)fixture->GetShape();
                        int num_edges = shape->GetChildCount();
                        for (int i_edge = 0; i_edge < num_edges; i_edge++) {
                            b2EdgeShape edge;
                            shape->GetChildEdge(&edge, i_edge);
                            auto b2_vert = edge.m_vertex1;
                            sf::Vertex vert(sf::Vector2f(b2_vert.x * PIXELS_PER_METER, b2_vert.y * PIXELS_PER_METER));
                            vert.color = sf::Color::Green;
                            verts.append(vert);
                        }
                        verts.append(verts[0]);

                    // Edge
                    } else if (fixture->GetType() == b2Shape::Type::e_edge) {
                        auto shape = (b2EdgeShape*)fixture->GetShape();
                        sf::Vertex a(sf::Vector2f(shape->m_vertex1.x * PIXELS_PER_METER, shape->m_vertex1.y * PIXELS_PER_METER));
                        sf::Vertex b(sf::Vector2f(shape->m_vertex2.x * PIXELS_PER_METER, shape->m_vertex2.y * PIXELS_PER_METER));
                        a.color = b.color = sf::Color::Green;
                        verts.append(a);
                        verts.append(b);
                    }

                    // Draw the vertices
                    window->draw(verts, sf::RenderStates(t * t_body));
                }
            }
        }


        // Render the foreground
        // TODO
    }

};
