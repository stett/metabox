#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "frame/Component.h"
using std::string;

class Texture : public frame::Component {
 protected:
    sf::Texture texture;

 public:
    Texture() { set("placeholder.png"); }
    Texture(string filename) { set(filename); }

 public:
    void set(string filename) { texture.loadFromFile(filename); }
    const sf::Texture& get() const { return texture; }
};
