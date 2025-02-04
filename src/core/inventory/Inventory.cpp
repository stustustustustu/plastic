#include "Inventory.h"

Inventory::Inventory(Entity &player) : player(player) {}

int Inventory::getCoins() const {
    return player.getCoins();
}

bool Inventory::hasEnoughCoins(int amount) const {
    return player.getCoins() >= amount;
}

bool Inventory::spendCoins(int amount) const {
    return player.spendCoins(amount);
}

void Inventory::addCoins(int amount) {
    player.addCoins(amount);
}
