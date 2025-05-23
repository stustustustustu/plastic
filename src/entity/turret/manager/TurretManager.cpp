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

void TurretManager::initUpgrades() { // temp
    // laser
    Upgrade* laserDamageUpgrade1 = new Upgrade("Laser Damage I", 50, TURRET_DAMAGE, 1.1f, "Increases laser damage by 10%");
    Upgrade* laserDamageUpgrade2 = new Upgrade("Laser Damage II", 100, TURRET_DAMAGE, 1.2f, "Increases laser damage by 20%");
    Upgrade* laserFireRateUpgrade = new Upgrade("Laser Fire Rate", 75, TURRET_FIRERATE, 1.15f, "Increases laser fire rate by 15%");
    Upgrade* laserRangeUpgrade = new Upgrade("Laser Range", 100, TURRET_RANGE, 1.2f, "Increases laser range by 20%");
    Upgrade* laserSlowUpgrade = new Upgrade("Laser Slow", 120, TURRET_SPECIAL, 0.8f, "Slows enemies hit by laser");

    upgradeManager.addUpgrade(TurretType::LASER, laserDamageUpgrade1);
    upgradeManager.addUpgrade(TurretType::LASER, laserDamageUpgrade2, { laserDamageUpgrade1 });
    upgradeManager.addUpgrade(TurretType::LASER, laserFireRateUpgrade, { laserDamageUpgrade1 }, { laserRangeUpgrade });
    upgradeManager.addUpgrade(TurretType::LASER, laserRangeUpgrade, { laserDamageUpgrade1 }, { laserFireRateUpgrade });
    upgradeManager.addUpgrade(TurretType::LASER, laserSlowUpgrade, { laserDamageUpgrade2 });

    // rifle
    Upgrade* rifleDamageUpgrade1 = new Upgrade("Rifle Damage I", 50, TURRET_DAMAGE, 1.1f, "Increases rifle damage by 10%");
    Upgrade* rifleDamageUpgrade2 = new Upgrade("Rifle Damage II", 100, TURRET_DAMAGE, 1.2f, "Increases rifle damage by 20%");
    Upgrade* rifleFireRateUpgrade = new Upgrade("Rifle Fire Rate", 75, TURRET_FIRERATE, 1.15f, "Increases rifle fire rate by 15%");
    Upgrade* riflePierceUpgrade = new Upgrade("Rifle Pierce", 120, TURRET_SPECIAL, 1.0f, "Bullets pierce through enemies");

    upgradeManager.addUpgrade(TurretType::RIFLE, rifleDamageUpgrade1);
    upgradeManager.addUpgrade(TurretType::RIFLE, rifleDamageUpgrade2, { rifleDamageUpgrade1 });
    upgradeManager.addUpgrade(TurretType::RIFLE, rifleFireRateUpgrade, { rifleDamageUpgrade1 }, { riflePierceUpgrade });
    upgradeManager.addUpgrade(TurretType::RIFLE, riflePierceUpgrade, { rifleDamageUpgrade1 }, { rifleFireRateUpgrade });

    // bomb
    Upgrade* bombDamageUpgrade1 = new Upgrade("Bomb Damage I", 50, TURRET_DAMAGE, 1.1f, "Increases bomb damage by 10%");
    Upgrade* bombDamageUpgrade2 = new Upgrade("Bomb Damage II", 100, TURRET_DAMAGE, 1.2f, "Increases bomb damage by 20%");
    Upgrade* bombRangeUpgrade = new Upgrade("Bomb Range", 100, TURRET_RANGE, 1.2f, "Increases bomb range by 20%");
    Upgrade* bombBurnUpgrade = new Upgrade("Bomb Burn", 120, TURRET_SPECIAL, 1.0f, "Enemies hit by bombs burn over time");

    upgradeManager.addUpgrade(TurretType::BOMB, bombDamageUpgrade1);
    upgradeManager.addUpgrade(TurretType::BOMB, bombDamageUpgrade2, { bombDamageUpgrade1 });
    upgradeManager.addUpgrade(TurretType::BOMB, bombRangeUpgrade, { bombDamageUpgrade1 }, { bombBurnUpgrade });
    upgradeManager.addUpgrade(TurretType::BOMB, bombBurnUpgrade, { bombDamageUpgrade1 }, { bombRangeUpgrade });
}

std::map<Upgrade*, glm::vec2> TurretManager::calculateNodePositions() const {
    std::map<Upgrade*, glm::vec2> nodePositions;
    float xOffset = 64.0f;
    float yOffset = 32.0f;

    int row = 0;
    int col = 0;
    for (const auto& upgrade : upgradeManager.getAvailableUpgrades(selectedTurret -> getType())) {
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

std::vector<std::shared_ptr<Turret>> TurretManager::getTurrets() const {
    return this -> turrets;
}

void TurretManager::addTurret(TurretType type, const glm::vec2& position) {
    auto turret = std::make_shared<Turret>(position, type);
    turrets.push_back(turret);
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

    game -> renderer -> DrawRect(
        glm::vec2(gridX, gridY),
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

void TurretManager::placeTurret(const glm::vec2& position) {
    if (!isPlacingTurret || !isValidPlacement(position)) return;

    if (game -> getCurrentWorld() -> inventory -> hasEnoughCoins(Turret::getCost(placingTurretType))) {
        if (game -> getCurrentWorld() -> inventory -> spendCoins(Turret::getCost(placingTurretType))) {
            addTurret(placingTurretType, position);

            Event event;
            event.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - game -> getCurrentWorld() -> replay -> getStartTime()
            );
            event.type = EventType::TURRET_PLACE;

            event.data.resize(sizeof(TurretType) + sizeof(glm::vec2));
            memcpy(event.data.data(), &placingTurretType, sizeof(TurretType));
            memcpy(event.data.data() + sizeof(TurretType), &position, sizeof(glm::vec2));

            game -> getCurrentWorld() -> replay -> addEvent(event);
        }
    } else {
        std::cout << "Not enough coins to place turret." << std::endl;
    }

    isPlacingTurret = false;
}

void TurretManager::clearTurrets() {
    this -> turrets.clear();
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
        if (Collision::isPointInRectangle({ worldPos.x, worldPos.y }, { turret -> getPosition().x + 16 / 2, turret -> getPosition().y + 16 / 2 }, {16, 16}) && glfwGetMouseButton(game -> window, GLFW_MOUSE_BUTTON_1)) {
            // openUpgradeMenu(turret);
            return;
        }
    }

    selectedTurret = NULL;
}

void TurretManager::openUpgradeMenu(std::shared_ptr<Turret> turret) {
    this -> selectedTurret = turret;
    this -> isUpgradeMenuOpen = true;

    auto turretPos = turret -> getPosition();
    auto screenCenter = glm::vec2(game -> getSize().x / 2, game -> getSize().y / 2);

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
           Collision::isPointInRectangle({ worldPos.x, worldPos.y }, { selectedTurret -> getPosition().x + 16 / 2, selectedTurret -> getPosition().y + 16 / 2 }, {16, 16});
}


bool TurretManager::isValidPlacement(const glm::vec2& position) const {
    int tileX = static_cast<int>(position[0] / Island::TILE_SIZE);
    int tileY = static_cast<int>(position[1] / Island::TILE_SIZE);

    if (!game -> getCurrentWorld() -> island -> isLand(tileX, tileY)) {
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