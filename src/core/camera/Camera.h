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
        const float maxZoom = 1.0f;

        bool smoothMovement = true;

        void ApplyConstraints();

    public:
        Camera(float screenWidth, float screenHeight);

        void Update();
        void Move(glm::vec2 delta);
        void Zoom(float amount);

        glm::mat4 getProjectionMatrix() const;

        glm::vec2 getPosition() const;
        void setPosition(const glm::vec2& position);

        float getZoom() const;
        void setZoom(float zoom);

        void setSmoothMovement(bool);
};



#endif //CAMERA_H
