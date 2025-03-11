#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "../action/manager/ActionManager.h"
#include "../src/config.h"

class InputHandler {
    private:
        std::vector<std::tuple<std::vector<int>, std::string, bool>> bindings;
        std::unordered_map<std::string, bool> keyPressedState;
        ActionManager actionManager;

    public:
        InputHandler();

        void InitBindings();
        void RegisterActions();

        void bindKeyCombo(const std::vector<int>& keys, const std::string& actionName, bool debounce = false);

        void processInput();

        static glm::vec2 getMousePosition();

        ActionManager& getActionManager();
};

#endif //INPUTHANDLER_H
