#ifndef SCENE_H
#define SCENE_H

#include "../src/config.h"
#include "../elements/Element.h"

enum SceneType {
    GAME,
    MENU,
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
        void setElementAction(int index, const Action::Callback &callback) const;
};

#endif //SCENE_H
