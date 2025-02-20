#ifndef PLAYER_H
#define PLAYER_H

#include "../src/config.h"
#include "../Entity.h"
#include "../enemy/Enemy.h"
#include "../projectile/Projectile.h"

class Player : public Entity {
    private:
        static bool shooting;
        static std::vector<float> laserStart;
        static std::vector<float> laserEnd;

    public:
        Player();

        std::pair<int, int> calculateSpawnTile();

        static void Movement();
        static void shoot();

        static void drawLaser();

        static bool canMove(std::vector<float> &delta);

        static bool isMouseOver(double mouseX, double mouseY);
};

#endif //PLAYER_H
