#ifndef SCENE_H
#define SCENE_H

#include "../src/config.h"
#include "../elements/Element.h"

class Scene {
    private:
        std::string sceneID;

    protected:
        std::vector<std::unique_ptr<Element>> elements;

    public:
        Scene(const std::string &sceneID) : sceneID(sceneID) {}
        virtual ~Scene() = default;

        virtual void render() = 0;
        virtual void update() = 0;
        virtual void resize() = 0;

        std::string getSceneID() const;
        void setSceneID(const std::string &scene_id);

        void addElement(std::unique_ptr<Element> element);
        void removeElement(const Element* element);

        const std::vector<std::unique_ptr<Element>>& getElements() const;
        void setElementAction(int index, const Action::Callback &callback) const;
};

#endif //SCENE_H
