#ifndef PLAYER_H
#define PLAYER_H

#include "../src/config.h"
#include "../Entity.h"
#include "../enemy/Enemy.h"

class Player : public Entity {
    public:
        static void Movement();
        static bool canMove(std::vector<float> &delta);

        static void drawTargetLine(std::vector<Enemy>& enemies);

        static bool isMouseOver(double mouseX, double mouseY);
};

#endif //PLAYER_H
