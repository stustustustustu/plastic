#include "Action.h"

void Action::addCallback(const Callback &callback) {
    callbacks.push_back(callback);
}

void Action::execute() {
    for (auto& callback : callbacks) {
        callback();
    }
}
