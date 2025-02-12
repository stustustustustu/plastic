#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "../src/config.h"
#include "../../Element.h"

class Textbox : public Element{
    private:
        std::string text;

    public:
        Textbox(glm::vec2 position, glm::vec2 size, std::string text) : Element(position, size), text(text) {}

        void render() override;
        void update() override {}
};



#endif //TEXTBOX_H
