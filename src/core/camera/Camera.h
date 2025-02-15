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

        const float minZoom = 0.5f;
        const float maxZoom = 2.0f;

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
};



#endif //CAMERA_H
