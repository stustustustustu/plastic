#ifndef Entity_H
#define Entity_H

#include "../src/config.h"
#include "../utils/render/Renderer.h"
#include "../utils/math/collision/Collision.h"
#include "../utils/math/color/color.h"


class Entity {
    private:
        // Position vector
        std::vector<float> position;

        // Entity attributes with default values
        float speed;
        float damage;

        float health;
        float maxHealth;

        float shield;
        float maxShield;

        int level;
        int coins;

        int targets;
        int maxTargets;
        std::vector<std::vector<float>> bounds;

    public:
        Entity(
            std::vector<float> position,

            float speed = 1.0f,
            float damage = 1.0f,

            float health = 100.0f,
            float maxHealth = 100.0f,

            float shield = 50.0f,
            float maxShield = 50.0f,

            int level = 1,
            int coins = 100,

            int targets = 0,
            int maxTargets = 1,

            std::vector<std::vector<float>> bounds = {{0, 0}, {32, 0}, {32, 32}, {0, 32}}
        );

        void drawEntity(Texture *texture) const;

        void drawTargetLine(Entity target, float thickness, int hex) const;

        std::vector<std::vector<float>> getBounds() const;
        void setBounds(const std::vector<std::vector<float>>&);
        bool checkCollision(const Entity&) const;
        void updateBounds();

        std::vector<float> getPosition() const;
        std::vector<int> getRenderPosition() const;
        void setPosition(std::vector<float>); // Set position to given vector
        void move(std::vector<int>); // Add vectors together
        void move(const std::vector<float>&);

        float getSpeed() const;
        void setSpeed(float);

        float getDamage() const;
        void setDamage(float);

        float getHealth() const;
        void setHealth(float);

        float getMaxHealth() const;
        void setMaxHealth(float);

        float getShield() const;
        void setShield(float);

        float getMaxShield() const;
        void setMaxShield(float);

        void hit(float, bool); // Entity taking damage
        void heal(float, bool); // Entity healing

        int getLevel() const;
        void setLevel(int);

        int getCoins() const;
        void setCoins(int);
        void addCoins(int);
        bool spendCoins(int);
        bool takeCoins(Entity&, float);
};

#endif //Entity_H
