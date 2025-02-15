#include "Shop.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

TurretShop::TurretShop(glm::vec2 position, glm::vec2 size) : Container(position, size) {
    auto laserTurret = new Button(position + glm::vec2(10, 10), glm::vec2(150, 50), "Buy Laser Turret");
    addElement(laserTurret);

    auto rifleTurret = new Button(position + glm::vec2(10, 70), glm::vec2(150, 50), "Buy Rifle Turret");
    addElement(rifleTurret);

    auto bombTurret = new Button(position + glm::vec2(10, 130), glm::vec2(150, 50), "Buy Bomb Turret");
    addElement(bombTurret);
}

void TurretShop::render() {
    game -> renderer -> DrawSpriteSheet(*game -> texture, getPosition(), 2, 32, 32, getSize(), 0, HEXtoRGB(0x404040));
    Container::render();

    /*if (waiting) {
        game -> renderer -> DrawText("Placing...", getPosition() + glm::vec2(10, 130), 20.0f, 0, HEXtoRGB(0xFFFFFF));
    }*/
}

void TurretShop::update() {
    Container::update();

    for (auto &element : elements) {
        if (auto button = dynamic_cast<Button*>(element)) {
            if (button -> isClicked()) {
                if (button == elements.at(0)) {
                    selectedType = TurretType::LASER;
                    //std::cout << "Waiting for placement of LASER..." << std::endl;
                } else if (button == elements.at(1)) {
                    selectedType = TurretType::RIFLE;
                    //std::cout << "Waiting for placement of RIFLE..." << std::endl;
                } else if (button == elements.at(2)) {
                    selectedType = TurretType::BOMB;
                    //std::cout << "Waiting for placement of BOMB..." << std::endl;
                }
            }
        }
    }
}