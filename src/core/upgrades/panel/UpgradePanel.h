#ifndef UPGRADEPANEL_H
#define UPGRADEPANEL_H

#include "../Upgrade.h"
#include "../../UI/elements/button/Button.h"

class UpgradePanel {
    private:
        Upgrade upgrade;
        glm::vec2 position;
        glm::vec2 size;
        Button button;

        float border = 8;

    public:
        UpgradePanel(const Upgrade& upgrade, glm::vec2 position, glm::vec2 size);

        void render();
        void update();

        void setPosition(glm::vec2 position);
        glm::vec2 getSize() const;
};

#endif //UPGRADEPANEL_H
