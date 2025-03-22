#ifndef PLAYER_H
#define PLAYER_H

#include "../src/config.h"
#include "../Entity.h"
#include "../enemy/Enemy.h"
#include "../projectile/Projectile.h"

class Player : public Entity {
    private:
        static bool shooting;
        static glm::vec2 laserStart;
        static glm::vec2 laserEnd;

    public:
        Player();

        std::pair<int, int> calculateSpawnTile();

        void Movement();
        void shoot(const glm::vec2& cursorPos);

        void drawLaser();

        bool canMove(glm::vec2 &delta);

        bool isMouseOver(double mouseX, double mouseY);
};

#endif //PLAYER_H
