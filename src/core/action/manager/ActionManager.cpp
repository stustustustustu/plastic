#include "ActionManager.h"

void ActionManager::registerAction(const std::string &name, const Action::Callback& callback) {
    actions[name].addCallback(callback);
}

void ActionManager::executeAction(const std::string &name) {
    if (actions.find(name) != actions.end()) {
        actions[name].execute();
    }
}
