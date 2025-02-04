#ifndef INVENTORY_H
#define INVENTORY_H

#include "../../config.h"
#include "../../entity/Entity.h"

class Inventory {
    public:
        explicit Inventory(Entity& player);

        int getCoins() const;
        bool hasEnoughCoins(int amount) const;
        bool spendCoins(int amount) const;
        void addCoins(int amount);

    private:
        Entity &player;
};

#endif //INVENTORY_H
