#include "Inventory.h"
#include "../src/Game.h"

const auto game = Game::getInstance();

int Inventory::getCoins() const {
    return game -> getCurrentWorld() -> player -> getCoins();
}

bool Inventory::hasEnoughCoins(int amount) const {
    return game -> getCurrentWorld() -> player -> getCoins() >= amount;
}

bool Inventory::spendCoins(int amount) const {
    return game -> getCurrentWorld() -> player -> spendCoins(amount);
}

void Inventory::addCoins(int amount) {
    game -> getCurrentWorld() -> player -> addCoins(amount);
}
