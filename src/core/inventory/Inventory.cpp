#include "Inventory.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

int Inventory::getCoins() const {
    return game -> player -> getCoins();
}

bool Inventory::hasEnoughCoins(int amount) const {
    return game -> player -> getCoins() >= amount;
}

bool Inventory::spendCoins(int amount) const {
    return game -> player -> spendCoins(amount);
}

void Inventory::addCoins(int amount) {
    game -> player -> addCoins(amount);
}
