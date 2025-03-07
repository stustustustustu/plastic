#include "ShopPanel.h"

#include "../src/Game.h"

const auto game = Game::getInstance();

constexpr auto border = 8;
constexpr auto portrait = 48;

ShopPanel::ShopPanel(glm::vec2 position, glm::vec2 size, TurretType type)
    : position(position), size(size),
      button(position + glm::vec2(border, portrait + border + 6), glm::vec2(24, 24), "", HEXtoRGB(0x6F6F6F)),
      type(type) {

    button.addCallback([this]() {
        if (game -> player -> getCoins() >= Turret::getCost(this -> type)) {
            game -> turret -> startPlacingTurret(this -> type);
        }
    });
}

void ShopPanel::render() {
    // panel shadow
    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(border / 2 + 2), 2, 32, 32, size - glm::vec2(border, 0), 0, HEXtoRGB(0x000000));

    // panel
    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(border / 2), 2, 32, 32, size - glm::vec2(border, 0), 0, HEXtoRGB(0x4F4F4F));

    // turret image
    int hex;
    std::ostringstream damage, fireRate, cost;
    std::string name;
    switch (type) {
        case TurretType::LASER:
            hex = 0xFF5733;
            damage << std::fixed << std::setprecision(1) << 1.0f;
            fireRate << std::fixed << std::setprecision(1) << 10.0f;
            cost << "$" << Turret::getCost(TurretType::LASER);
            name = "LASER";
            break;
        case TurretType::RIFLE:
            hex = 0xF2EE0A;
            damage << std::fixed << std::setprecision(1) << 5.0f;
            fireRate << std::fixed << std::setprecision(1) << 20.0f;
            cost << "$" << Turret::getCost(TurretType::RIFLE);
            name = "RIFLE";
            break;
        case TurretType::BOMB:
            hex = 0x323232;
            damage << std::fixed << std::setprecision(1) << 25.0f;
            fireRate << std::fixed << std::setprecision(1) << 2.0f;
            cost << "$" << Turret::getCost(TurretType::BOMB);
            name = "BOMB";
            break;
    }

    game -> renderer -> DrawSpriteSheet(*game -> texture, position + glm::vec2(border, border), 2, 32, 32, glm::vec2(portrait), 0, HEXtoRGB(hex));

    // text
    game -> renderer -> DrawText(name, position + glm::vec2(portrait + 2 * border, 16 + border / 2), 16.0f, true, HEXtoRGB(hex)); // name

    game -> renderer -> DrawText("Damage: " + damage.str(), position + glm::vec2(portrait + 2 * border, 24 + border / 2 + 4), 8.0f, true, HEXtoRGB(0xFFFFFF)); // damage
    game -> renderer -> DrawText("Fire Rate: " + fireRate.str(), position + glm::vec2(portrait + 2 * border, 32 + border / 2 + 4 * 2), 8.0f, true, HEXtoRGB(0xFFFFFF)); // fire rate
    game -> renderer -> DrawText(cost.str(), position + size - glm::vec2(game -> text -> GetWidth(cost.str(), 16.0f) + border, 2), 16.0f, true, HEXtoRGB(0xFDFF74)); // cost

    // button
    button.render();
}

void ShopPanel::update() {
    if (game -> player -> getCoins() < Turret::getCost(type)) {
        button.setActive(false);
    } else {
        button.setActive(true);
    }

    button.update();
}

void ShopPanel::setPosition(glm::vec2 position) {
    this -> position = position;
    button.setPosition(position + glm::vec2(border, portrait + border + 6));
}

glm::vec2 ShopPanel::getSize() const {
    return this -> size;
}