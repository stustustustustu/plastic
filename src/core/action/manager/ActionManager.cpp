#include "ActionManager.h"

void ActionManager::registerAction(const std::string &name, const Action::Callback& callback) {
    actions[name].addCallback(callback);
}

void ActionManager::executeAction(const std::string &name) {
    if (actions.find(name) != actions.end()) {
        actions[name].execute();
    }
}

bool ActionManager::getActionState(const std::string& name) const {
    if (states.find(name) != states.end()) {
        return states.at(name);
    }
    return false;
}

void ActionManager::setActionState(const std::string& name, bool state) {
    states[name] = state;
}