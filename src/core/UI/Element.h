#ifndef ELEMENT_H
#define ELEMENT_H

#include "../src/config.h"

class Element {
    protected:
        glm::vec2 position;
        glm::vec2 size;
public:
    Element(glm::vec2 position, glm::vec2 size) : position(position), size(size) {}
    virtual ~Element() = default;

    virtual void render() = 0;

    void setPosition(glm::vec2);
    glm::vec2 getPosition() const;

    void setSize(glm::vec2);
    glm::vec2 getSize() const;
};



#endif //ELEMENT_H
