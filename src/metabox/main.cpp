#include "frame/Frame.h"
#include "systems/Physics.hpp"
#include "systems/GameWindow.hpp"
#include "systems/Window.hpp"
#include "systems/GameEntityBoxTraversal.hpp"
#include "components/BoxNode.h"
#include "components/ViewFollow.h"
#include "components/BackgroundTexture.h"
#include "components/GameEntity.h"
#include "components/Texture.h"
#include "entity_factories.hpp"

int main(int argc, char** argv) {

    // Make frame
    frame::Frame f;

    // Add a window system
    //f.add_system<Physics>();
    //f.add_system<RenderBoxes>();
    f.add_system<GameWindow>();
    f.add_system<GameEntityBoxTraversal>();

    // Add the root box entity
    auto root_box = box_entity(&f);
    root_box->add_component<ViewFollow>();

    // Add some child-boxes
    auto box0 = box_entity(&f, root_box, 1, 0);
    auto box1 = box_entity(&f, root_box, 2, 3);
    auto box2 = box_entity(&f, box1, 2, 2);

    // Add a player object to the root box
    auto player = game_entity(&f, root_box)->add_component<ViewFollow>();
    auto bird = game_entity(&f, box1);
    auto jacob = game_entity(&f, box2);

    // Run the program
    f.run();

    return 0;
}
