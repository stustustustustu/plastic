#include "Game.h"
Game game(1280, 1280 * 3/4); // 4:3 format

int main() {
    if (!game.Init()) {
        return -1;
    }

    game.Loop();

    return 0;
}