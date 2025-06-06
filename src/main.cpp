#include "Game.h"
auto *game = Game::getInstance(1280, 1280 * 3/4); // 4:3 format

int main() {
    if (!game -> init()) {
        return -1;
    }

    game -> loop();

    delete game;

    return 0;
}