#ifndef SCENE_H
#define SCENE_H

#include "../src/config.h"
#include "../UI/Element.h"

enum SceneType {
    MENU,
    GAME,
    GAME_OVER,
    REPLAY
};

class Scene {
    private:
        SceneType type;
        std::vector<std::unique_ptr<Element>> elements;

    public:
        Scene(SceneType type) : type(type) {}
        virtual ~Scene() = default;

        virtual void render() = 0;
        virtual void update() = 0;

        SceneType getType() const;
        void setType(SceneType type);

        void addElement(std::unique_ptr<Element> element);

        void removeElement(const Element* element);

        const std::vector<std::unique_ptr<Element>>& getElements() const;
};

#endif //SCENE_H
