#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "frame/Component.h"
using std::string;

class BackgroundTexture : public frame::Component {
 protected:
    sf::Texture texture;

 public:
    BackgroundTexture() { set("placeholder.png"); }
    BackgroundTexture(string filename) { set(filename); }

 public:
    void set(string filename) { texture.loadFromFile(filename); }
    const sf::Texture& get() const { return texture; }
};
