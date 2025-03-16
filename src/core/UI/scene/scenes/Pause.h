#ifndef PAUSE_H
#define PAUSE_H

#include "../Scene.h"
#include "../../elements/button/Button.h"

class Pause : public Scene {
    private:
        std::unique_ptr<Button> resume;
        std::unique_ptr<Button> restart;
        std::unique_ptr<Button> settings;
        std::unique_ptr<Button> exit;

    public:
        Pause();

        void render() override;
        void update() override;
        void resize() override;

};

#endif //PAUSE_H
