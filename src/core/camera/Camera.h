#ifndef CAMERA_H
#define CAMERA_H

#include "../src/config.h"
#include <glm/ext/matrix_clip_space.hpp>

class Camera {
    private:
        glm::vec2 position;
        glm::vec2 targetPosition;

        float zoom;
        float targetZoom;

        float screenWidth, screenHeight;

        const float minZoom = 1.0f;
        const float maxZoom = 3.0f; // to be changed

        bool smoothMovement = true;

    public:
        bool panning = false;
        glm::dvec2 initialMousePos;

    public:
        Camera(float screenWidth, float screenHeight);

        void Update();
        void Move(glm::vec2 delta);
        void Zoom(float amount);

        glm::mat4 getCameraProjection() const;
        glm::mat4 getStaticProjection() const;
        void projectionMatrixToText(glm::mat4 projection) const;

        glm::vec2 screenToWorld(glm::vec2 screenPos) const;

        // panning
        void handlePanning(double mouseX, double mouseY, float sensitivity);
        void startPanning(double mouseX, double mouseY);
        void stopPanning();

        glm::vec2 getPosition() const;
        void setPosition(const glm::vec2& position);

        float getZoom() const;
        void setZoom(float zoom);

        glm::vec2 getScreen() const;

        void setSmoothMovement(bool);

        void returnToDefault();

        void setSize(int screenWidth, int screenHeight);
};



#endif //CAMERA_H
