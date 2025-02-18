#ifndef PLAYER_H
#define PLAYER_H

#include "../src/config.h"
#include "../Entity.h"
#include "../enemy/Enemy.h"
#include "../projectile/Projectile.h"

class Player : public Entity {
    public:
        Player();

        std::pair<int, int> calculateSpawnTile();

        static void Movement();
        static void shoot();

        static bool canMove(std::vector<float> &delta);

        static bool isMouseOver(double mouseX, double mouseY);
};

#endif //PLAYER_H
