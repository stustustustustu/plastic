#include "Game.h"
Game game(1280, 720);

int main() {
    if (!game.Init()) {
        return -1;
    }

    game.Loop();

    return 0;
}