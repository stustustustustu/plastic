#include "Player.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

bool Player::shooting = false;
glm::vec2 Player::laserStart = {0, 0};
glm::vec2 Player::laserEnd = {0, 0};

Player::Player() : Entity({0, 0}) {}

std::pair<int, int> Player::calculateSpawnTile() {
    int screenCenterX = game -> getSize().x / 2;
    int screenCenterY = game -> getSize().y / 2;

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
    glm::vec2 delta {0, 0};

    // Movement logic
    if (game -> input -> getActionManager().getActionState("UP")) {
        delta.y -= 1; // Up
    }
    if (game -> input -> getActionManager().getActionState("DOWN")) {
        delta.y += 1; // Down
    }
    if (game -> input -> getActionManager().getActionState("LEFT")) {
        delta.x -= 1; // Left
    }
    if (game -> input -> getActionManager().getActionState("RIGHT")) {
        delta.x += 1; // Right
    }

    float magnitude = glm::length(delta);
    if (magnitude > 0) {
        delta /= magnitude;
    }

    // Shooting
    if (game -> input -> getActionManager().getActionState("SHOOT")) {
        glm::vec2 cursorWorldPos = game -> camera -> screenToWorld(game->input->getMousePosition());

        shoot(cursorWorldPos);

        Event event;

        event.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - game -> getCurrentWorld() -> replay -> getStartTime()
        );

        event.type = EventType::PLAYER_SHOOT;
        event.data.resize(sizeof(glm::vec2));
        memcpy(event.data.data(), &laserEnd, sizeof(glm::vec2));
        game -> getCurrentWorld() -> replay -> addEvent(event);
    } else {
        shooting = false;
    }

    if (canMove(delta)) {
        auto pos1 = getPosition();
        game -> getCurrentWorld() -> player -> move(delta);
        auto pos2 = getPosition();

        if (pos1 != pos2) {
            Event event;

            event.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - game -> getCurrentWorld() -> replay -> getStartTime()
            );
            event.type = EventType::PLAYER_MOVE;

            glm::vec2 position = getPosition();
            event.data.resize(sizeof(glm::vec2));
            memcpy(event.data.data(), &position, sizeof(glm::vec2));

            game -> getCurrentWorld() -> replay -> addEvent(event);
        }
    }
}

void Player::shoot(const glm::vec2& cursorPos) {
    shooting = true;

    laserStart = {
        getPosition().x + 16,
        getPosition().y + 16
    };

    laserEnd = {
        cursorPos.x,
        cursorPos.y
    };

    if (game -> getCurrentWorld()) {
        for (auto& enemy : *game -> getCurrentWorld() -> enemies) {
            if (Collision::lineRectangleIntersection(game -> getCurrentWorld() -> player -> getPosition() + glm::vec2(16), laserEnd, enemy.getHitboxCenter(), enemy.getHitboxSize() / 2.0f)) {
                enemy.hit(getDamage() * 0.5f, false);
            }
        }
    }
}

void Player::drawLaser() {
    if (!shooting) return;

    game -> renderer -> DrawLine(
        laserStart,
        laserEnd,
        2.0f,
        glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
    );
}

bool Player::canMove(glm::vec2& delta) {
    glm::vec2 position = game -> getCurrentWorld() -> player -> getPosition();
    const float speed = game -> getCurrentWorld() -> player -> getSpeed();

    const float newX = position.x + delta.x * speed;
    const float newY = position.y + delta.y * speed;

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
    if (delta.x != 0) {
        xValid = checkPosition(newX, position.y) && checkPosition(newX, position.y + 31) &&
                 checkPosition(newX + 31, position.y) && checkPosition(newX + 31, position.y + 31);
    }

    bool yValid = true;
    if (delta.y != 0) {
        yValid = checkPosition(position.x, newY) && checkPosition(position.x + 31, newY) &&
                 checkPosition(position.x, newY + 31) && checkPosition(position.x + 31, newY + 31);
    }

    if (!xValid) delta.x = 0;
    if (!yValid) delta.y = 0;

    return xValid || yValid;
}