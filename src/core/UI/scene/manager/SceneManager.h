#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "../Scene.h"

#include <stack>

class SceneManager {
    private:
        std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;
        Scene* currentScene = NULL;

        std::stack<std::string> sceneHistory;

    public:
        void addScene(const std::string &sceneID, std::unique_ptr<Scene> scene);

        void switchScene(const std::string &sceneID);
        void goBack();

        void render();
        void update();
        void resize();

        void initScene() const;
        std::string getScene() const;

        void clearHistory();
};

#endif //SCENEMANAGER_H
