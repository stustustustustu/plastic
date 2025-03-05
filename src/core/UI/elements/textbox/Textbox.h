#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "../src/config.h"
#include "../Element.h"

class Textbox : public Element {
    private:
        std::string text;
        std::string placeholder;
        bool focused = false;

        std::unordered_map<int, std::chrono::time_point<std::chrono::high_resolution_clock>> keyLastPressTimes;

    public:
        Textbox(glm::vec2 position, glm::vec2 size, const std::string& placeholder);
        void render() override;
        void update() override;

        std::string getText() const;
        void setText(const std::string& text);

    private:
        bool debounce(int key, float debounceTime);
};

#endif //TEXTBOX_H
