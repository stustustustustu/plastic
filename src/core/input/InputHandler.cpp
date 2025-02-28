#include "InputHandler.h"
#include "../../Game.h"

const auto game = Game::getInstance();

InputHandler::InputHandler() {
    InitBindings();
    RegisterActions();
}

void InputHandler::InitBindings() {
    // player movement
    bindKeyCombo( {GLFW_KEY_W}, "UP");
    bindKeyCombo( {GLFW_KEY_S}, "DOWN");
    bindKeyCombo( {GLFW_KEY_A}, "LEFT");
    bindKeyCombo( {GLFW_KEY_D}, "RIGHT");
    bindKeyCombo( {GLFW_MOUSE_BUTTON_1}, "SHOOT");
    bindKeyCombo( {GLFW_MOUSE_BUTTON_2}, "PLACE_TURRET");

    bindKeyCombo( {GLFW_KEY_ESCAPE}, "CANCEL_PLACEMENT");

    // camera
    bindKeyCombo( {GLFW_MOUSE_BUTTON_3}, "CAMERA_PAN");
    bindKeyCombo( {GLFW_KEY_UP}, "CAMERA_ZOOM_IN");
    bindKeyCombo( {GLFW_KEY_DOWN}, "CAMERA_ZOOM_OUT");
}

void InputHandler::RegisterActions() {
    // player movement
    getActionManager().registerAction("UP",[](){  });
    getActionManager().registerAction("DOWN", [](){  });
    getActionManager().registerAction("LEFT", [](){  });
    getActionManager().registerAction("RIGHT", [](){  });
    getActionManager().registerAction("SHOOT", [](){  });
    getActionManager().registerAction("PLACE_TURRET", []() {
        game -> turret -> handleClick(getMousePosition());
    });

    getActionManager().registerAction("CANCEL_PLACEMENT", []() {
        game -> turret -> cancelPlacingTurret();
    });

    // camera
    getActionManager().registerAction("CAMERA_PAN", []() {
        if (!game -> camera -> panning) {
            auto mousePos = getMousePosition();
            game -> camera -> startPanning(mousePos.x, mousePos.y);
        }
    });

    getActionManager().registerAction("CAMERA_ZOOM_IN", []() {
        game -> camera -> Zoom(-0.01f);
    });

    getActionManager().registerAction("CAMERA_ZOOM_OUT", []() {
        game -> camera -> Zoom(0.01f);
    });
}

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

        actionManager.setActionState(binding.second, allPressed);
    }

    // camera panning
    if (game -> camera -> panning && glfwGetMouseButton(game -> window, GLFW_MOUSE_BUTTON_3) != GLFW_PRESS) {
        game -> camera -> stopPanning();
    }
}

glm::vec2 InputHandler::getMousePosition() {
    double mouseX, mouseY;
    glfwGetCursorPos(game -> window, &mouseX, &mouseY);

    return glm::vec2(mouseX, mouseY);
}

ActionManager & InputHandler::getActionManager() {
    return this -> actionManager;
}
