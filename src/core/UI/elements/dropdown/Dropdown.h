#ifndef DROPDOWN_H
#define DROPDOWN_H

#include "../src/config.h"
#include "../Element.h"

class Dropdown : public Element {
    private:
        std::vector<std::string> options;
        int selectedIndex = 0;
        bool open = false;

    public:
        Dropdown(glm::vec2 position, glm::vec2 size, const std::string& label);
        void render() override;
        void update() override;

        bool isOpen() const;

        void addOption(const std::string& option);
        int getOptionCount() const;

        int getSelectedIndex() const;
        void selectOption(int index);
};

#endif //DROPDOWN_H
