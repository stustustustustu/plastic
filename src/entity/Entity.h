#ifndef Entity_H
#define Entity_H

#include "../core/upgrades/Upgrade.h"
#include "../src/config.h"
#include "../utils/render/Renderer.h"
#include "../utils/math/collision/Collision.h"
#include "../utils/math/color/color.h"

class Entity {
    private:
        // Position vector
        glm::vec2 position;

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
        std::vector<glm::vec2> bounds;

    public:
        Entity(
            glm::vec2 position,

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

            std::vector<glm::vec2> bounds = {{0, 0}, {32, 0}, {32, 32}, {0, 32}}
        );

        void drawEntity(Texture *texture) const;

        void drawTargetLine(Entity target, float thickness, int hex) const;

        std::vector<glm::vec2> getBounds() const;
        void setBounds(const std::vector<glm::vec2>&);
        bool checkCollision(const Entity&) const;
        void updateBounds();

        glm::vec2 getPosition() const;
        glm::ivec2 getRenderPosition() const;
        void setPosition(glm::vec2);
        void move(const glm::ivec2&);
        void move(const glm::vec2&);

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

        void hit(float, bool);
        void heal(float, bool);

        int getLevel() const;
        void setLevel(int);

        int getCoins() const;
        void setCoins(int);
        void addCoins(int);
        bool spendCoins(int);
        bool takeCoins(Entity&, float);

        void applyUpgrade(Upgrade upgrade);

};

#endif //Entity_H
