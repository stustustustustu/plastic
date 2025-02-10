#ifndef COLLISION_H
#define COLLISION_H

#include <../src/config.h>

#include <cmath>

class Collision {
    public:
        // vector basics
        static std::vector<float> addVectors(const std::vector<float>& v1, const std::vector<float>& v2);
        static std::vector<float> subtractVectors(const std::vector<float>& v1, const std::vector<float>& v2);
        static std::vector<float> multiplyVectorByScalar(const std::vector<float>& v, float scalar);
        static std::vector<float> divideVectorByScalar(const std::vector<float>& v, float scalar);
        static float dotProduct(const std::vector<float>& v1, const std::vector<float>& v2);
        static float magnitude(const std::vector<float>& v);
        static std::vector<float> normalize(const std::vector<float>& v);

        // collision detection and distance
        static float distance(const std::vector<float>& p1, const std::vector<float>& p2);
        static bool isPointInRectangle(const std::vector<float>& p, const std::vector<float>& center, const std::vector<float>& halfDimensions);
        static bool satCollision(const std::vector<std::vector<float>>& A, const std::vector<std::vector<float>>& B);

    private:
        // helpers
        static std::vector<std::vector<float>> getAxes(const std::vector<std::vector<float>>& polygon);
        static bool projectAndCheckOverlap(const std::vector<std::vector<float>>& A, const std::vector<std::vector<float>>& B, const std::vector<float>& axis);
        static void projectPolygon(const std::vector<std::vector<float>>& polygon, const std::vector<float>& axis, float& minProj, float& maxProj);
};



#endif //COLLISION_H
