#ifndef SHOP_H
#define SHOP_H

#include "../../../../core/UI/elements/button/Button.h"
#include "../../../../core/UI/elements/container/Container.h"
#include "../../Turret.h"

class TurretShop : public Container {
    private:
        bool waiting = false; // waiting for turret to be placed
        TurretType selectedType;

    public:
        TurretShop(glm::vec2 position, glm::vec2 size);

        void render() override;

        void update() override;

        bool isWaiting() const {
            return this -> waiting;
        }

        TurretType getSelectedType() const {
            return this -> selectedType;
        }

        void placeTurret(const glm::vec2 &position) {
            waiting = false;
            std::cout << "Placed " << (selectedType == TurretType::LASER? "LASER" : selectedType == TurretType::RIFLE? "RIFLE" : "BOMB") << " turret at (" << position.x << ", " << position.y << ")" << std::endl;
        }
};

#endif //SHOP_H
