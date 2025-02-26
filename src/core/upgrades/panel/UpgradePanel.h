#ifndef UPGRADEPANEL_H
#define UPGRADEPANEL_H

#include "../Upgrade.h"
#include "../../UI/elements/button/Button.h"

class UpgradePanel {
    public:
        using Refresh = std::function<void()>;

    private:
        Upgrade upgrade;
        glm::vec2 position;
        glm::vec2 size;
        Button button;

        Refresh refresh;

        float border = 8;

    public:
        UpgradePanel(const Upgrade &upgrade, glm::vec2 position, glm::vec2 size, Refresh refresh);

        void render();
        void update();

        void setPosition(glm::vec2 position);
        glm::vec2 getSize() const;

};

#endif //UPGRADEPANEL_H
