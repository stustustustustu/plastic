#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Scene.h"

class SceneManager {
    private:
        std::unordered_map<SceneType, std::unique_ptr<Scene>> scenes;
        Scene* currentScene = NULL;

    public:
        void addScene(SceneType type, std::unique_ptr<Scene> scene);

        void switchScene(SceneType type);

        void render();
        void update();
        void resize();

        void initScene() const;
        SceneType getScene() const;
};

#endif //SCENEMANAGER_H
