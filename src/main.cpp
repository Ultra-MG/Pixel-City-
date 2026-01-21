// main.cpp
#include "core/Game.hpp"

int main() {
    Game game;
    game.run();   // Game owns clear / draw / display
    return 0;
}
