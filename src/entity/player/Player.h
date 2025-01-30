#ifndef PLAYER_H
#define PLAYER_H

#include "../Entity.h"

class Player : Entity {
    public:
        static void Movement(Entity& player, GLFWwindow* window, std::vector<Entity>& enemies);
        static bool canMove(Entity &player, GLFWwindow *window, std::vector<float> &delta);

        static void drawTargetLine(Entity& player, GLFWwindow* window, std::vector<Entity>& enemies);

        static bool isKeyPressed(GLFWwindow* window, int key);
        static bool isMousePressed(GLFWwindow* window, int key);
        static bool isMouseOver(GLFWwindow* window, double mouseX, double mouseY);
};

#endif //PLAYER_H
