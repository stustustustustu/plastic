#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "../action/manager/ActionManager.h"
#include "../src/config.h"

class InputHandler {
    private:
        std::vector<std::pair<std::vector<int>, std::string>> bindings;
        ActionManager actionManager;

    public:
        void bindKeyCombo(const std::vector<int>& keys, const std::string& actionName);

        void processInput();

        ActionManager& getActionManager();
};

#endif //INPUTHANDLER_H
