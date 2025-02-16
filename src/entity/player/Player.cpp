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

    if (canMove(delta)) {
        game -> player.move(delta);
    }
}

bool Player::canMove(std::vector<float>& delta) {
    std::vector<float> position = game -> player.getPosition();
    const float speed = game -> player.getSpeed();

    const float newX = position[0] + delta[0] * speed;
    const float newY = position[1] + delta[1] * speed;

    auto checkPosition = [&](float x, float y) {
        int tileX = static_cast<int>(x) / Island::TILE_SIZE;
        int tileY = static_cast<int>(y) / Island::TILE_SIZE;
        if (!game -> generator -> isLand(tileX, tileY)) {
            return false;
        }

        bool beach = game -> generator -> isWater(tileX - 1, tileY) || game -> generator -> isWater(tileX + 1, tileY) ||
                     game -> generator -> isWater(tileX, tileY - 1) || game -> generator -> isWater(tileX, tileY + 1);

        if (!beach) return true;

        float posInTileX = x - tileX * Island::TILE_SIZE;
        float posInTileY = y - tileY * Island::TILE_SIZE;

        bool valid = true;
        if (game -> generator -> isWater(tileX - 1, tileY)) valid &= (posInTileX >= 16);
        if (game -> generator -> isWater(tileX + 1, tileY)) valid &= (posInTileX <= 16);
        if (game -> generator -> isWater(tileX, tileY - 1)) valid &= (posInTileY >= 16);
        if (game -> generator -> isWater(tileX, tileY + 1)) valid &= (posInTileY <= 16);

        return valid;
    };

    bool xValid = true;
    if (delta[0] != 0) {
        xValid = checkPosition(newX, position[1]) && checkPosition(newX, position[1] + 31) &&
                 checkPosition(newX + 31, position[1]) && checkPosition(newX + 31, position[1] + 31);
    }

    bool yValid = true;
    if (delta[1] != 0) {
        yValid = checkPosition(position[0], newY) && checkPosition(position[0] + 31, newY) &&
                 checkPosition(position[0], newY + 31) && checkPosition(position[0] + 31, newY + 31);
    }

    if (!xValid) delta[0] = 0;
    if (!yValid) delta[1] = 0;

    return xValid || yValid;
}

bool Player::isMouseOver(double x, double y) {
    double mouseX, mouseY;

    glfwGetCursorPos(game -> window, &mouseX, &mouseY);

    return mouseX >= x && mouseX <= x + 32 && mouseY >= y && mouseY <= y + 32;
}