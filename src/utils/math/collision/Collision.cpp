#include "Collision.h"
#include <glm/glm.hpp>
#include <cmath>

glm::vec2 Collision::addVectors(const glm::vec2& v1, const glm::vec2& v2) {
    return v1 + v2;
}

glm::vec2 Collision::subtractVectors(const glm::vec2& v1, const glm::vec2& v2) {
    return v1 - v2;
}

glm::vec2 Collision::multiplyVectorByScalar(const glm::vec2& v, float scalar) {
    return v * scalar;
}

glm::vec2 Collision::divideVectorByScalar(const glm::vec2& v, float scalar) {
    return v / scalar;
}

float Collision::dotProduct(const glm::vec2& v1, const glm::vec2& v2) {
    return glm::dot(v1, v2);
}

float Collision::magnitude(const glm::vec2& v) {
    return glm::length(v);
}

glm::vec2 Collision::normalize(const glm::vec2& v) {
    return glm::normalize(v);
}

float Collision::distance(const glm::vec2& p1, const glm::vec2& p2) {
    return glm::length(p1 - p2);
}

bool Collision::isPointInRectangle(const glm::vec2& p, const glm::vec2& center, const glm::vec2& halfDimensions) {
    return (std::abs(p.x - center.x) <= halfDimensions.x &&
            std::abs(p.y - center.y) <= halfDimensions.y);
}

bool Collision::lineRectangleIntersection(const glm::vec2& lineStart, const glm::vec2& lineEnd, const glm::vec2& rectCenter, const glm::vec2& rectHalfDimensions) {
    float rectLeft = rectCenter.x - rectHalfDimensions.x;
    float rectRight = rectCenter.x + rectHalfDimensions.x;
    float rectTop = rectCenter.y - rectHalfDimensions.y;
    float rectBottom = rectCenter.y + rectHalfDimensions.y;

    float x1 = lineStart.x;
    float y1 = lineStart.y;
    float x2 = lineEnd.x;
    float y2 = lineEnd.y;

    if (isPointInRectangle(lineStart, rectCenter, rectHalfDimensions)) return true;
    if (isPointInRectangle(lineEnd, rectCenter, rectHalfDimensions)) return true;

    if (x1 < rectLeft && x2 < rectLeft) return false; // left
    if (x1 > rectRight && x2 > rectRight) return false; // right
    if (y1 < rectTop && y2 < rectTop) return false; // top
    if (y1 > rectBottom && y2 > rectBottom) return false; // bottom

    // youtube
    auto lineIntersectsEdge = [](float x1, float y1, float x2, float y2, float edgeX1, float edgeY1, float edgeX2, float edgeY2) -> bool {
        float dxLine = x2 - x1;
        float dyLine = y2 - y1;
        float dxEdge = edgeX2 - edgeX1;
        float dyEdge = edgeY2 - edgeY1;

        float det = dxLine * dyEdge - dyLine * dxEdge;

        if (det == 0) {
            return false;
        }

        float t = ((edgeX1 - x1) * dyEdge - (edgeY1 - y1) * dxEdge) / det;
        float u = ((edgeX1 - x1) * dyLine - (edgeY1 - y1) * dxLine) / det;

        return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
    };

    if (lineIntersectsEdge(x1, y1, x2, y2, rectLeft, rectTop, rectLeft, rectBottom)) return true; // left
    if (lineIntersectsEdge(x1, y1, x2, y2, rectRight, rectTop, rectRight, rectBottom)) return true; // right
    if (lineIntersectsEdge(x1, y1, x2, y2, rectLeft, rectTop, rectRight, rectTop)) return true; // top
    if (lineIntersectsEdge(x1, y1, x2, y2, rectLeft, rectBottom, rectRight, rectBottom)) return true; // bottom

    return false;
}

bool Collision::satCollision(const std::vector<glm::vec2>& A, const std::vector<glm::vec2>& B) {
    if (A.empty() || B.empty()) {
        return false;
    }

    std::vector<glm::vec2> axes = getAxes(A);
    std::vector<glm::vec2> axesB = getAxes(B);
    axes.insert(axes.end(), axesB.begin(), axesB.end());

    for (const auto& axis : axes) {
        float minA, maxA, minB, maxB;
        projectPolygon(A, axis, minA, maxA);
        projectPolygon(B, axis, minB, maxB);

        if (!(minA <= maxB && minB <= maxA)) {
            return false;
        }
    }

    return true;
}

bool Collision::AABBCollision(const std::vector<glm::vec2>& A, const std::vector<glm::vec2>& B) {
    if (A.empty() || B.empty()) return false;

    float minA_x = A[0].x;
    float minA_y = A[0].y;
    float maxA_x = A[1].x;
    float maxA_y = A[1].y;

    float minB_x = B[0].x;
    float minB_y = B[0].y;
    float maxB_x = B[1].x;
    float maxB_y = B[1].y;

    return (minA_x <= maxB_x && maxA_x >= minB_x && minA_y <= maxB_y && maxA_y >= minB_y);
}

std::vector<glm::vec2> Collision::getAxes(const std::vector<glm::vec2>& polygon) {
    std::vector<glm::vec2> axes;

    for (size_t i = 0; i < polygon.size(); i++) {
        glm::vec2 edge = subtractVectors(polygon[(i + 1) % polygon.size()], polygon[i]);
        glm::vec2 normal = normalize(glm::vec2(-edge.y, edge.x));
        if (magnitude(normal) > 0) { // Ensure the normal is valid
            axes.push_back(normal);
        }
    }

    return axes;
}

void Collision::projectPolygon(const std::vector<glm::vec2>& polygon, const glm::vec2& axis, float& minProj, float& maxProj) {
    if (polygon.empty()) {
        minProj = 0;
        maxProj = 0;
        return;
    }

    float minDot = std::numeric_limits<float>::max();
    float maxDot = std::numeric_limits<float>::lowest();

    for (const auto& vertex : polygon) {
        float dotValue = dotProduct(vertex, axis);
        minDot = std::min(minDot, dotValue);
        maxDot = std::max(maxDot, dotValue);
    }

    minProj = minDot;
    maxProj = maxDot;
}