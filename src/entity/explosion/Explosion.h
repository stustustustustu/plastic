#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "../Entity.h"

class Explosion : public Entity {
    private:
        float size;
        float damage;
        float duration;

        float elapsedTime;

    public:
        Explosion(std::vector<float> position, float size, float damage, float duration);

        void update();
        void render() const;
        bool isFinished() const;

    private:
        void applySplashDamage() const;
};

#endif //EXPLOSION_H
