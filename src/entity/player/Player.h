#ifndef PLAYER_H
#define PLAYER_H

#include "../Entity.h"
#include "../enemy/Enemy.h"

class Player : public Entity {
    public:
        static void Movement(Renderer *renderer, Entity& player, GLFWwindow* window, std::vector<Enemy>& enemies);
        static bool canMove(Entity &player, GLFWwindow *window, std::vector<float> &delta);

        static void drawTargetLine(Entity& player, GLFWwindow* window, std::vector<Enemy>& enemies);

        static bool isKeyPressed(GLFWwindow* window, int key);
        static bool isMousePressed(GLFWwindow* window, int key);
        static bool isMouseOver(GLFWwindow* window, double mouseX, double mouseY);
};

#endif //PLAYER_H
