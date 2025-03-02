#include "TurretManager.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

TurretManager::TurretManager() {
    initUpgrades();
}

void TurretManager::update() {
    for (auto& turret : turrets) {
        turret -> findTarget();
        turret -> rotateTowardsTarget();
        turret -> shoot();
    }
}

void TurretManager::render() {
    game -> renderer -> SetProjection(game -> camera -> getCameraProjection());

    for (const auto& turret : turrets) {
        turret -> render(game -> texture);
    }

    if (isPlacing()) {
        auto mousePos = game -> input -> getMousePosition();
        renderPreview(mousePos);
    }

    game -> renderer -> SetProjection(game -> camera -> getStaticProjection());
}

void TurretManager::initUpgrades() {
    Upgrade* laserDamageUpgrade = new Upgrade("Laser Damage I", 50, TURRET_DAMAGE, 1.1f, "Increases laser damage by 10%");
    Upgrade* laserFireRateUpgrade = new Upgrade("Laser Fire Rate I", 75, TURRET_FIRERATE, 1.15f, "Increases laser fire rate by 15%");
    Upgrade* laserRangeUpgrade = new Upgrade("Laser Range I", 100, TURRET_RANGE, 1.2f, "Increases laser range by 20%");

    upgradeManager.addUpgrade(laserDamageUpgrade);
    upgradeManager.addUpgrade(laserFireRateUpgrade, { laserDamageUpgrade });

    upgradeManager.addUpgrade(laserRangeUpgrade, { laserDamageUpgrade, laserFireRateUpgrade });

    Upgrade* rifleDamageUpgrade = new Upgrade("Rifle Damage I", 50, TURRET_DAMAGE, 1.1f, "Increases rifle damage by 10%");
    Upgrade* rifleFireRateUpgrade = new Upgrade("Rifle Fire Rate I", 75, TURRET_FIRERATE, 1.15f, "Increases rifle fire rate by 15%");

    upgradeManager.addUpgrade(rifleDamageUpgrade);
    upgradeManager.addUpgrade(rifleFireRateUpgrade, { rifleDamageUpgrade });

    Upgrade* bombDamageUpgrade = new Upgrade("Bomb Damage I", 50, TURRET_DAMAGE, 1.1f, "Increases bomb damage by 10%");
    Upgrade* bombRangeUpgrade = new Upgrade("Bomb Range I", 100, TURRET_RANGE, 1.2f, "Increases bomb range by 20%");

    upgradeManager.addUpgrade(bombDamageUpgrade);
    upgradeManager.addUpgrade(bombRangeUpgrade, { bombDamageUpgrade });
}

std::map<Upgrade*, glm::vec2> TurretManager::calculateNodePositions() const {
    std::map<Upgrade*, glm::vec2> nodePositions;
    float xOffset = 64.0f;
    float yOffset = 32.0f;

    int row = 0;
    int col = 0;
    for (const auto& upgrade : upgradeManager.getAvailableUpgrades()) {
        glm::vec2 position = menuPosition + glm::vec2(0, menuSize.y / 4) + glm::vec2(col * xOffset, row * yOffset);
        nodePositions[upgrade] = position;

        col++;
        if (col > 2) {
            col = 0;
            row++;
        }
    }

    return nodePositions;
}

void TurretManager::renderPreview(const glm::vec2& mousePos) const {
    if (!isPlacingTurret) return;

    auto worldPos = game -> camera -> screenToWorld(mousePos);

    int gridX = static_cast<int>(worldPos[0] / Island::TILE_SIZE) * Island::TILE_SIZE;
    int gridY = static_cast<int>(worldPos[1] / Island::TILE_SIZE) * Island::TILE_SIZE;

    bool isValid = isValidPlacement({static_cast<float>(gridX), static_cast<float>(gridY)});

    int color;
    switch (placingTurretType) {
        case TurretType::LASER:
            color = 0xFF5733;
            break;
        case TurretType::RIFLE:
            color = 0xF2EE0A;
            break;
        case TurretType::BOMB:
            color = 0x323232;
            break;
    }

    float opacity = isValid ? 0.5f : 0.1f;

    game -> renderer -> DrawSpriteSheet(
        *game -> texture,
        glm::vec2(gridX, gridY),
        2, 32, 32,
        glm::vec2(32.0f),
        0.0f,
        HEXtoRGB(color),
        opacity
    );
}

void TurretManager::startPlacingTurret(TurretType type) {
    isPlacingTurret = true;
    placingTurretType = type;
}

void TurretManager::placeTurret(const std::vector<float>& position) {
    if (!isPlacingTurret || !isValidPlacement(position)) return;

    if (game -> inventory -> hasEnoughCoins(Turret::getCost(placingTurretType))) {
        if (game -> inventory -> spendCoins(Turret::getCost(placingTurretType))) {
            auto turret = std::make_shared<Turret>(position, placingTurretType);
            turrets.push_back(turret);
        }
    } else {
        std::cout << "Not enough coins to place turret." << std::endl;
    }

    isPlacingTurret = false;
}

void TurretManager::cancelPlacingTurret() {
    isPlacingTurret = false;
}

void TurretManager::handleClick(const glm::vec2& mousePos) {
    if (isPlacingTurret && glfwGetMouseButton(game -> window, GLFW_MOUSE_BUTTON_2)) {
        auto worldPos = game -> camera -> screenToWorld(mousePos);

        int gridX = static_cast<int>(worldPos[0] / Island::TILE_SIZE) * Island::TILE_SIZE;
        int gridY = static_cast<int>(worldPos[1] / Island::TILE_SIZE) * Island::TILE_SIZE;

        placeTurret({static_cast<float>(gridX), static_cast<float>(gridY)});
        return;
    }

    if (isMouseInsideMenu(mousePos)) {

        return;
    } else if (isUpgrading()){
        closeUpgradeMenu();
        return;
    }

    auto worldPos = game -> camera -> screenToWorld(mousePos);

    for (const auto &turret : turrets) {
        auto turretPos = turret -> getPosition();
        if (Collision::isPointInRectangle({ worldPos.x, worldPos.y }, { turret -> getPosition().at(0) + 16 / 2, turret -> getPosition().at(1) + 16 / 2 }, {16, 16}) && glfwGetMouseButton(game -> window, GLFW_MOUSE_BUTTON_1)) {
            openUpgradeMenu(turret);
            return;
        }
    }

    selectedTurret = NULL;
}

void TurretManager::openUpgradeMenu(std::shared_ptr<Turret> turret) {
    this -> selectedTurret = turret;
    this -> isUpgradeMenuOpen = true;

    auto turretPos = turret -> getPosition();
    auto screenCenter = glm::vec2(game -> getSize().at(0) / 2, game -> getSize().at(1) / 2);

    menuSize = glm::vec2(400, 200);

    if (turretPos[0] < screenCenter.x) {
        menuPosition = glm::vec2(turretPos[0] + 40, turretPos[1] - 50);
    } else {
        menuPosition = glm::vec2(turretPos[0] - menuSize.x - 40, turretPos[1] - 50);
    }
}

void TurretManager::closeUpgradeMenu() {
    this -> isUpgradeMenuOpen = false;
    this -> selectedTurret = NULL;
}

bool TurretManager::isUpgrading() const {
    return this -> isUpgradeMenuOpen;
}

bool TurretManager::isMouseInsideMenu(const glm::vec2 &mousePos) const {
    if (!isUpgrading()) return false;

    auto worldPos = game -> camera -> screenToWorld(mousePos);

    return Collision::isPointInRectangle({mousePos.x, mousePos.y}, {menuPosition.x + menuSize.x / 2, menuPosition.y + menuSize.y / 2}, {menuSize.x / 2, menuSize.y / 2}) ||
           Collision::isPointInRectangle({ worldPos.x, worldPos.y }, { selectedTurret -> getPosition().at(0) + 16 / 2, selectedTurret -> getPosition().at(1) + 16 / 2 }, {16, 16});
}


bool TurretManager::isValidPlacement(const std::vector<float>& position) const {
    int tileX = static_cast<int>(position[0] / Island::TILE_SIZE);
    int tileY = static_cast<int>(position[1] / Island::TILE_SIZE);

    if (!game -> generator -> isLand(tileX, tileY)) {
        return false;
    }

    for (const auto& turret : turrets) {
        int turretTileX = static_cast<int>(turret -> getPosition()[0] / Island::TILE_SIZE);
        int turretTileY = static_cast<int>(turret -> getPosition()[1] / Island::TILE_SIZE);

        if (abs(turretTileX - tileX) <= 1 && abs(turretTileY - tileY) <= 1) {
            return false;
        }
    }

    for (const auto& turret : turrets) {
        int turretTileX = static_cast<int>(turret -> getPosition()[0] / Island::TILE_SIZE);
        int turretTileY = static_cast<int>(turret -> getPosition()[1] / Island::TILE_SIZE);

        if (abs(turretTileX - tileX) <= 1 && abs(turretTileY - tileY) <= 1) {
            return false;
        }
    }

    return true;
}

bool TurretManager::isPlacing() const {
    return this -> isPlacingTurret;
}

std::shared_ptr<Turret> TurretManager::getSelectedTurret() const {
    return this -> selectedTurret;
}

TurretUpgradeManager& TurretManager::getUpgradeManager() {
    return this -> upgradeManager;
}

bool TurretManager::unlockTurretUpgrade(const std::string& upgradeName) {
    if (selectedTurret) {
        if (upgradeManager.unlockUpgrade(*selectedTurret, upgradeName)) {
            std::cout << "Upgrade unlocked: " << upgradeName << std::endl;
            return true;
        } else {
            std::cout << "Cannot unlock upgrade: " << upgradeName << std::endl;
        }
    }
    return false;
}