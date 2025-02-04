#include "../Entity.h"
#include "Player.h"
#include "../src/config.h"

void Player::Movement(Entity& player, GLFWwindow* window, std::vector<Entity>& enemies) {
    std::vector<float> delta {0, 0};

    // Movement logic
    if (isKeyPressed(window, GLFW_KEY_W)) {
        delta.at(1) -= 1; // Up
    }
    if (isKeyPressed(window, GLFW_KEY_S)) {
        delta.at(1) += 1; // Down
    }
    if (isKeyPressed(window, GLFW_KEY_A)) {
        delta.at(0) -= 1; // Left
    }
    if (isKeyPressed(window, GLFW_KEY_D)) {
        delta.at(0) += 1; // Right
    }

    float magnitude = std::sqrt(delta[0] * delta[0] + delta[1] * delta[1]);

    if (magnitude > 0) {
        delta[0] = delta[0] / magnitude;
        delta[1] = delta[1] / magnitude;
    }

    // Shooting
    if (isMousePressed(window, GLFW_MOUSE_BUTTON_1)) {
        for (auto it = enemies.begin(); it != enemies.end();) {
            Entity& enemy = *it;
            if (isMouseOver(window, enemy.getPosition().at(0), enemy.getPosition().at(1))) {
                enemy.hit(player.getDamage(), false);
            }
            ++it;
        }
    }

    if (canMove(player, window, delta)) {
        player.move(delta);
    }
}

bool Player::canMove(Entity& player, GLFWwindow* window, std::vector<float>& delta) {
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    const int halfWidth = 16;
    const int halfHeight = 16;
    const int border = 4;

    std::vector<float> position = player.getPosition();

    float newX = position[0] + delta[0] * player.getSpeed();
    float newY = position[1] + delta[1] * player.getSpeed();

    // Horizontal check
    if (newX < border) {
        delta[0] = std::max(0.0f, delta[0]); // Left
    } else if (newX + 2 * halfWidth + border > screenWidth) {
        delta[0] = std::min(0.0f, delta[0]); // Right
    }

    // Vertical check
    if (newY < border) {
        delta[1] = std::max(0.0f, delta[1]); // Up
    } else if (newY + 2 * halfHeight + border > screenHeight) {
        delta[1] = std::min(0.0f, delta[1]); // Down
    }

    return true;
}

bool Player::isKeyPressed(GLFWwindow* window, int key) {
    int state = glfwGetKey(window, key);
    return state == GLFW_PRESS;
}

bool Player::isMousePressed(GLFWwindow* window, int key) {
    int state = glfwGetMouseButton(window, key);
    return state == GLFW_PRESS;
}

bool Player::isMouseOver(GLFWwindow* window, double x, double y) {
    double mouseX, mouseY;

    glfwGetCursorPos(window, &mouseX, &mouseY);

    return mouseX >= x && mouseX <= x + 32 && mouseY >= y && mouseY <= y + 32;
}