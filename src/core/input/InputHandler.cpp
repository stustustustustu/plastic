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

    // turret
    bindKeyCombo( {GLFW_MOUSE_BUTTON_2}, "PLACE_TURRET", true);
    bindKeyCombo( {GLFW_KEY_ESCAPE}, "CANCEL_PLACEMENT", true);

    // camera
    bindKeyCombo( {GLFW_MOUSE_BUTTON_3}, "CAMERA_PAN");
    bindKeyCombo( {GLFW_KEY_UP}, "CAMERA_ZOOM_IN");
    bindKeyCombo( {GLFW_KEY_DOWN}, "CAMERA_ZOOM_OUT");

    //bindKeyCombo( {GLFW_KEY_ESCAPE}, "PAUSE");
}

void InputHandler::RegisterActions() {
    // player movement
    getActionManager().registerAction("UP",[](){  });
    getActionManager().registerAction("DOWN", [](){  });
    getActionManager().registerAction("LEFT", [](){  });
    getActionManager().registerAction("RIGHT", [](){  });
    getActionManager().registerAction("SHOOT", []() {
        if (game -> getCurrentWorld() && game -> getCurrentWorld() -> turret) {
            game -> getCurrentWorld() -> turret -> handleClick(getMousePosition());
        }
    });

    getActionManager().registerAction("PLACE_TURRET", []() {
        if (game -> getCurrentWorld()) {
            game -> getCurrentWorld() -> turret -> handleClick(getMousePosition());
        }
    });

    getActionManager().registerAction("CANCEL_PLACEMENT", []() {
        if (game -> getCurrentWorld() && game -> getCurrentWorld() -> turret -> isPlacing()) {
            game -> getCurrentWorld() -> turret -> cancelPlacingTurret();
        } else {
            game -> handlePause();
        }
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

void InputHandler::bindKeyCombo(const std::vector<int>& keys, const std::string& actionName, bool debounce) {
    for (const auto& binding : bindings) {
        if (std::get<0>(binding) == keys) {
            std::cerr << "Error: Key combination already used for action: " << std::get<1>(binding) << std::endl;
            return;
        }
    }
    bindings.push_back({keys, actionName, debounce});
}

void InputHandler::processInput() {
    for (const auto& binding : bindings) {
        const auto& keys = std::get<0>(binding);
        const auto& actionName = std::get<1>(binding);
        bool debounce = std::get<2>(binding);

        bool allPressed = true;

        for (int key : keys) {
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

        if (debounce) {
            bool wasPressed = keyPressedState[actionName];
            if (allPressed && !wasPressed) {
                actionManager.executeAction(actionName);
            }
            keyPressedState[actionName] = allPressed;
        } else {
            if (allPressed) {
                actionManager.executeAction(actionName);
            }
        }

        actionManager.setActionState(actionName, allPressed);
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
