#ifndef SHOPPANEL_H
#define SHOPPANEL_H

#include "../Turret.h"
#include "../../../core/UI/elements/button/Button.h"

class ShopPanel {
    private:
        glm::vec2 position;
        glm::vec2 size;
        Button button;
        TurretType type;

    public:
        ShopPanel(glm::vec2 position, glm::vec2 size, TurretType type);

        void render();
        void update();

        void setPosition(glm::vec2 position);
        glm::vec2 getSize() const;
};

#endif // SHOPPANEL_H