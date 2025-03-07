#include "Player.h"

#include "../src/Game.h"

const auto game = Game::getInstance();

bool Player::shooting = false;
std::vector<float> Player::laserStart = {0, 0};
std::vector<float> Player::laserEnd = {0, 0};

Player::Player() : Entity({0, 0}) {}

std::pair<int, int> Player::calculateSpawnTile() {
    int screenCenterX = game -> getSize().at(0) / 2;
    int screenCenterY = game -> getSize().at(1) / 2;

    auto [tileX, tileY] = game -> getCurrentWorld() -> island -> findNearestLandTile(screenCenterX, screenCenterY);

    bool isFullyEnclosed = true;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int neighborX = tileX + dx;
            int neighborY = tileY + dy;

            if (neighborX < 0 || neighborX >= game -> getCurrentWorld() -> island -> MAP_WIDTH ||
                neighborY < 0 || neighborY >= game -> getCurrentWorld() -> island -> MAP_HEIGHT ||
                !game -> getCurrentWorld() -> island -> isLand(neighborX, neighborY)) {
                isFullyEnclosed = false;
                break;
            }
        }
        if (!isFullyEnclosed) break;
    }

    if (isFullyEnclosed) {
        return {tileX, tileY};
    }

    int searchRadius = 1;
    int maxSearchRadius = std::max(game -> getCurrentWorld() -> island -> MAP_WIDTH, game -> getCurrentWorld() -> island -> MAP_HEIGHT);

    while (searchRadius < maxSearchRadius) {
        for (int dx = -searchRadius; dx <= searchRadius; ++dx) {
            for (int dy = -searchRadius; dy <= searchRadius; ++dy) {
                if (dx == 0 && dy == 0) continue;

                int newTileX = tileX + dx;
                int newTileY = tileY + dy;

                if (newTileX >= 0 && newTileX < game -> getCurrentWorld() -> island -> MAP_WIDTH &&
                    newTileY >= 0 && newTileY < game -> getCurrentWorld() -> island -> MAP_HEIGHT &&
                    game -> getCurrentWorld() -> island -> isLand(newTileX, newTileY)) {
                    bool isNewTileFullyEnclosed = true;
                    for (int nx = -1; nx <= 1; ++nx) {
                        for (int ny = -1; ny <= 1; ++ny) {
                            if (nx == 0 && ny == 0) continue;

                            int neighborX = newTileX + nx;
                            int neighborY = newTileY + ny;

                            if (neighborX < 0 || neighborX >= game -> getCurrentWorld() -> island -> MAP_WIDTH ||
                                neighborY < 0 || neighborY >= game -> getCurrentWorld() -> island -> MAP_HEIGHT ||
                                !game -> getCurrentWorld() -> island -> isLand(neighborX, neighborY)) {
                                isNewTileFullyEnclosed = false;
                                break;
                            }
                        }
                        if (!isNewTileFullyEnclosed) break;
                    }

                    if (isNewTileFullyEnclosed) {
                        return {newTileX, newTileY};
                    }
                }
            }
        }

        searchRadius++;
    }

    return {tileX, tileY};
}

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
        shoot();
    } else {
        shooting = false;
    }

    if (canMove(delta)) {
        game -> getCurrentWorld() -> player -> move(delta);
    }
}

void Player::shoot() {
    shooting = true;

    glm::vec2 cursorWorldPos = game -> camera -> screenToWorld(InputHandler::getMousePosition());

    laserStart = {
        game -> getCurrentWorld() -> player -> getPosition()[0] + 16,
        game -> getCurrentWorld() -> player -> getPosition()[1] + 16
    };

    laserEnd = {
        cursorWorldPos.x,
        cursorWorldPos.y
    };

    for (auto& enemy : *game -> getCurrentWorld() -> enemies) {
        std::vector<float> enemyCenter = {enemy.getPosition().at(0) + 16.0f, enemy.getPosition().at(1) + 16.0f};
        std::vector<float> enemyHalfDimensions = {16.0f, 16.0f}; // 32x32

        if (Collision::lineRectangleIntersection(laserStart, laserEnd, enemyCenter, enemyHalfDimensions)) {
            enemy.hit(game -> getCurrentWorld() -> player -> getDamage() * 0.5f, false);
        }
    }
}

void Player::drawLaser() {
    if (!shooting) return;

    game -> renderer -> DrawLine(
        glm::vec2(laserStart[0], laserStart[1]),
        glm::vec2(laserEnd[0], laserEnd[1]),
        2.0f,
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
}

bool Player::canMove(std::vector<float>& delta) {
    std::vector<float> position = game -> getCurrentWorld() -> player -> getPosition();
    const float speed = game -> getCurrentWorld() -> player -> getSpeed();

    const float newX = position[0] + delta[0] * speed;
    const float newY = position[1] + delta[1] * speed;

    auto checkPosition = [&](float x, float y) {
        int tileX = static_cast<int>(x) / Island::TILE_SIZE;
        int tileY = static_cast<int>(y) / Island::TILE_SIZE;
        if (!game -> getCurrentWorld() -> island -> isLand(tileX, tileY)) {
            return false;
        }

        bool beach = game -> getCurrentWorld() -> island -> isWater(tileX - 1, tileY) || game -> getCurrentWorld() -> island -> isWater(tileX + 1, tileY) ||
                     game -> getCurrentWorld() -> island -> isWater(tileX, tileY - 1) || game -> getCurrentWorld() -> island -> isWater(tileX, tileY + 1);

        if (!beach) return true;

        float posInTileX = x - tileX * Island::TILE_SIZE;
        float posInTileY = y - tileY * Island::TILE_SIZE;

        bool valid = true;
        if (game -> getCurrentWorld() -> island -> isWater(tileX - 1, tileY)) valid &= (posInTileX >= 16);
        if (game -> getCurrentWorld() -> island -> isWater(tileX + 1, tileY)) valid &= (posInTileX <= 16);
        if (game -> getCurrentWorld() -> island -> isWater(tileX, tileY - 1)) valid &= (posInTileY >= 16);
        if (game -> getCurrentWorld() -> island -> isWater(tileX, tileY + 1)) valid &= (posInTileY <= 16);

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