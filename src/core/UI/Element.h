#ifndef ELEMENT_H
#define ELEMENT_H

#include "../action/Action.h"
#include "../src/config.h"

class Element {
    protected:
        glm::vec2 position;
        glm::vec2 size; // width & height

        Action action;

        std::chrono::time_point<std::chrono::high_resolution_clock> lastActionTime; // debouncing

    public:
        Element(glm::vec2 position, glm::vec2 size) : position(position), size(size) {}
        virtual ~Element() = default;

        virtual void render() = 0;
        virtual void update() = 0;

        void setPosition(glm::vec2);
        glm::vec2 getPosition() const;

        void setSize(glm::vec2);
        glm::vec2 getSize() const;

        bool isHovering() const;
        bool isClicked() const;

        bool debounce(float debounce);

        void addCallback(const Action::Callback &callback);
        void executeAction();
};

#endif //ELEMENT_H
