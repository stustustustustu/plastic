#include "Camera.h"
#include "../../Game.h"

const auto game = Game::getInstance();

Camera::Camera(float screenWidth, float screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight), position(screenWidth, screenHeight), zoom(1.0f), targetZoom(1.0f), initialMousePos(0, 0) {}

void Camera::Update() {
    double mouseX, mouseY;
    glfwGetCursorPos(game -> window, &mouseX, &mouseY);
    handlePanning(mouseX, mouseY, 1.0f);

    if (smoothMovement) {
        position = glm::mix(position, targetPosition, 0.1f);
        zoom = glm::mix(zoom, targetZoom, 0.015f);
    } else {
        position = targetPosition;
        zoom = targetZoom;
    }

    if (glm::distance(position, glm::vec2(0.0f, 0.0f)) < 0.5f &&
        glm::abs(zoom - 1.0f) < 0.01f) {
        targetPosition = glm::vec2(0.0f, 0.0f);
        targetZoom = 1.0f;
    }

    // quick reset back to original position if camera isnt being interacted with (too fast to use)
    /*if (!(panning || game -> input -> getActionManager().getActionState("CAMERA_ZOOM_IN") || game -> input -> getActionManager().getActionState("CAMERA_ZOOM_OUT"))) {
        returnToDefault();
    }*/
}

void Camera::Move(glm::vec2 delta) {
    targetPosition += delta;
    ApplyConstraints();
}

void Camera::Zoom(float amount) {
    targetZoom += amount;
    targetZoom = glm::clamp(targetZoom, minZoom, maxZoom);
}

void Camera::ApplyConstraints() {
    float maxMovement = (screenWidth);
    targetPosition.x = glm::clamp(targetPosition.x, -maxMovement, maxMovement);
    targetPosition.y = glm::clamp(targetPosition.y, -maxMovement, maxMovement);
}

glm::mat4 Camera::getCameraProjection() const {
    float left = position.x - (screenWidth / 2.0f) * zoom;
    float right = position.x + (screenWidth / 2.0f) * zoom;
    float top = position.y - (screenHeight / 2.0f) * zoom;
    float bottom = position.y + (screenHeight / 2.0f) * zoom;

    return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

glm::mat4 Camera::getStaticProjection() const {
    return glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
}

void Camera::projectionMatrixToText(glm::mat4 projection) const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << std::setw(4) << projection[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Camera::startPanning(double mouseX, double mouseY) {
    panning = true;
    initialMousePos = glm::dvec2(mouseX, mouseY);
}

void Camera::stopPanning() {
    panning = false;
}

void Camera::handlePanning(double mouseX, double mouseY, float sensitivity) {
    if (panning) {
        glm::dvec2 currentMousePos(mouseX, mouseY);
        glm::dvec2 delta = initialMousePos - currentMousePos;

        Move(glm::vec2(delta) * sensitivity);

        initialMousePos = currentMousePos;
    }
}

glm::vec2 Camera::getPosition() const {
    return this -> position;
}

void Camera::setPosition(const glm::vec2& position) {
    this -> position = position;
    ApplyConstraints();
}

float Camera::getZoom() const {
    return this -> zoom;
}

void Camera::setZoom(float zoom) {
    this -> zoom = zoom;
    ApplyConstraints();
}

glm::vec2 Camera::getScreen() const {
    return glm::vec2(screenWidth, screenHeight);
}

void Camera::setSmoothMovement(bool enabled) {
    this -> smoothMovement = enabled;
}

void Camera::returnToDefault() {
    targetPosition = glm::vec2(screenWidth / 2.0f, screenHeight / 2.0f);
    targetZoom = 1.0f;
}