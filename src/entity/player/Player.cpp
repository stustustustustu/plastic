#include "Player.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

void Player::Movement() {
    std::vector<float> delta {0, 0};

    // Movement logic
    if (game -> input -> getActionManager().getActionState("UP")) {
        delta.at(1) -= 1; // Up
    }
    if (game -> input -> getActionManager().getActionState("DOWN")) {
        delta.at(1) += 1; // Down
    }
    if (game -> input -> getActionManager().getActionState("LEFT")) {
        delta.at(0) -= 1; // Left
    }
    if (game -> input -> getActionManager().getActionState("RIGHT")) {
        delta.at(0) += 1; // Right
    }

   float magnitude = Collision::magnitude(delta);
    if (magnitude > 0) {
        delta[0] /= magnitude;
        delta[1] /= magnitude;
    }

    // Shooting
    if (game -> input -> getActionManager().getActionState("SHOOT")) {
        for (auto it = game -> enemies -> begin(); it != game -> enemies -> end();) {
            Entity& enemy = *it;
            if (isMouseOver(enemy.getPosition().at(0), enemy.getPosition().at(1))) {
                enemy.hit(game -> player.getDamage(), false);
            }
            ++it;
        }
    }

    if (isMousePressed(GLFW_MOUSE_BUTTON_2)) {
        // temporary
    }

    if (canMove(delta)) {
        game -> player.move(delta);
    }
}

bool Player::canMove(std::vector<float>& delta) {
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(game -> window, &screenWidth, &screenHeight);

    const int halfWidth = 16;
    const int halfHeight = 16;
    const int border = 4;

    std::vector<float> position = game -> player.getPosition();

    float newX = position[0] + delta[0] * game -> player.getSpeed();
    float newY = position[1] + delta[1] * game -> player.getSpeed();

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

bool Player::isKeyPressed(int key) {
    int state = glfwGetKey(game -> window, key);
    return state == GLFW_PRESS;
}

bool Player::isMousePressed(int key) {
    int state = glfwGetMouseButton(game -> window, key);
    return state == GLFW_PRESS;
}

bool Player::isMouseOver(double x, double y) {
    double mouseX, mouseY;

    glfwGetCursorPos(game -> window, &mouseX, &mouseY);

    return mouseX >= x && mouseX <= x + 32 && mouseY >= y && mouseY <= y + 32;
}