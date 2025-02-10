#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight) : screenWidth(screenWidth), screenHeight(screenHeight), position(0.0f, 0.0f), zoom(1.0f), targetZoom(1.0f) {}

void Camera::Update() {
    if (smoothMovement) {
        position = glm::mix(position, targetPosition, 0.1f);
        zoom = glm::mix(zoom, targetZoom, 0.1f);
    } else {
        position = targetPosition;
        zoom = targetZoom;
    }
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
    float maxMovement = (screenWidth * 0.5f);
    targetPosition.x = glm::clamp(targetPosition.x, -maxMovement, maxMovement);
    targetPosition.y = glm::clamp(targetPosition.y, -maxMovement, maxMovement);
}

glm::mat4 Camera::getProjectionMatrix() const {
    float left = position.x - (screenWidth / 2.0f) * zoom;
    float right = position.x + (screenWidth / 2.0f) * zoom;
    float top = position.y - (screenHeight / 2.0f) * zoom;
    float bottom = position.y + (screenHeight / 2.0f) * zoom;

    return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
}

glm::vec2 Camera::getPosition() const {
    return this -> position;
}

float Camera::getZoom() const {
    return this -> zoom;
}

void Camera::setSmoothMovement(bool enabled) {
    this -> smoothMovement = enabled;
}
