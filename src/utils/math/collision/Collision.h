#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>
#include <vector>
#include <limits>

class Collision {
    public:
        // Vector basics
        static glm::vec2 addVectors(const glm::vec2& v1, const glm::vec2& v2);
        static glm::vec2 subtractVectors(const glm::vec2& v1, const glm::vec2& v2);
        static glm::vec2 multiplyVectorByScalar(const glm::vec2& v, float scalar);
        static glm::vec2 divideVectorByScalar(const glm::vec2& v, float scalar);
        static float dotProduct(const glm::vec2& v1, const glm::vec2& v2);
        static float magnitude(const glm::vec2& v);
        static glm::vec2 normalize(const glm::vec2& v);

        // Collision detection and distance
        static float distance(const glm::vec2& p1, const glm::vec2& p2);
        static bool isPointInRectangle(const glm::vec2& p, const glm::vec2& center, const glm::vec2& halfDimensions);
        static bool lineRectangleIntersection(const glm::vec2& lineStart, const glm::vec2& lineEnd, const glm::vec2& rectCenter, const glm::vec2& rectHalfDimensions);
        static bool satCollision(const std::vector<glm::vec2>& A, const std::vector<glm::vec2>& B);
        static bool AABBCollision(const std::vector<glm::vec2>& A, const std::vector<glm::vec2>& B);

    private:
        // Helpers
        static std::vector<glm::vec2> getAxes(const std::vector<glm::vec2>& polygon);
        static void projectPolygon(const std::vector<glm::vec2>& polygon, const glm::vec2& axis, float& minProj, float& maxProj);
};

#endif // COLLISION_H