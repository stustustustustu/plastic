#ifndef UPGRADEPATH_H
#define UPGRADEPATH_H

#include "../Upgrade.h"
#include "../../../config.h"

class UpgradePath {
    private:
        std::string name;
        std::vector<Upgrade> upgrades;
        int level = 0;
    public:
        UpgradePath(std::string &name, const std::vector<Upgrade> &upgrades);

        const std::string &getName() const;
        void setName(const std::string &name);

        const std::vector<Upgrade> &getUpgrades() const;
        const Upgrade &getCurrentUpgrade() const;
        const Upgrade &getNextUpgrade() const;
        bool canUpgrade() const;
        void upgrade();

        int getLevel() const;
        void setLevel(int level);
};

#endif //UPGRADEPATH_H
