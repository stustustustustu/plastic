#include "InputHandler.h"

#include "../../Game.h"

const auto game = Game::getInstance();

void InputHandler::bindKeyCombo(const std::vector<int>& keys, const std::string& actionName) {
    for (const auto& binding : bindings) {
        if (binding.first == keys) {
            std::cerr << "Error: Key combination already used for action: " << binding.second << std::endl;
            return;
        }
    }
    bindings.push_back({keys, actionName});
}

void InputHandler::processInput() {
    for (const auto& binding : bindings) {
        bool allPressed = true;

        for (int key : binding.first) {
            if (key >= GLFW_MOUSE_BUTTON_1 && key <= GLFW_MOUSE_BUTTON_LAST) {
                if (glfwGetMouseButton(game -> window, key) != GLFW_PRESS) {
                    allPressed = false;
                    break;
                }
            } else {
                if (glfwGetKey(game -> window, key) != GLFW_PRESS) {
                    allPressed = false;
                    break;
                }
            }
        }

        if (allPressed) {
            actionManager.executeAction(binding.second);
        }
    }
}

ActionManager & InputHandler::getActionManager() {
    return this -> actionManager;
}