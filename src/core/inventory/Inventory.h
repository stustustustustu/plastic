#ifndef INVENTORY_H
#define INVENTORY_H

#include "../../config.h"
#include "../../entity/player/Player.h"

class Inventory {
    public:
        explicit Inventory() = default;

        int getCoins() const;
        bool hasEnoughCoins(int amount) const;
        bool spendCoins(int amount) const;
        void addCoins(int amount);
};

#endif //INVENTORY_H
