#include "Collision.h"

std::vector<float> Collision::addVectors(const std::vector<float>& v1, const std::vector<float>& v2) {
    return {v1[0] + v2[0], v1[1] + v2[1]};
}

std::vector<float> Collision::subtractVectors(const std::vector<float>& v1, const std::vector<float>& v2) {
    return {v1[0] - v2[0], v1[1] - v2[1]};
}

std::vector<float> Collision::multiplyVectorByScalar(const std::vector<float>& v, float scalar) {
    return {v[0] * scalar, v[1] * scalar};
}

std::vector<float> Collision::divideVectorByScalar(const std::vector<float>& v, float scalar) {
    return {v[0] / scalar, v[1] / scalar};
}

float Collision::dotProduct(const std::vector<float>& v1, const std::vector<float>& v2) {
    return v1[0] * v2[0] + v1[1] * v2[1];
}

float Collision::magnitude(const std::vector<float>& v) {
    return std::sqrt(dotProduct(v, v));
}

std::vector<float> Collision::normalize(const std::vector<float>& v) {
    float len = magnitude(v);
    if (len > 0) return divideVectorByScalar(v, len);
    return v;
}

float Collision::distance(const std::vector<float> &p1, const std::vector<float> &p2) {
    std::vector<float> difference = subtractVectors(p1, p2);
    return magnitude(difference);
}

bool Collision::isPointInRectangle(const std::vector<float> &p, const std::vector<float> &center, const std::vector<float> &halfDimensions) {
    return (std::abs(p[0] - center[0]) <= halfDimensions[0] &&
            std::abs(p[1] - center[1]) <= halfDimensions[1]);
}

bool Collision::lineRectangleIntersection(const std::vector<float> &lineStart, const std::vector<float> &lineEnd, const std::vector<float> &rectCenter, const std::vector<float> &rectHalfDimensions) {
    float rectLeft = rectCenter[0] - rectHalfDimensions[0];
    float rectRight = rectCenter[0] + rectHalfDimensions[0];
    float rectTop = rectCenter[1] - rectHalfDimensions[1];
    float rectBottom = rectCenter[1] + rectHalfDimensions[1];

    float x1 = lineStart[0];
    float y1 = lineStart[1];
    float x2 = lineEnd[0];
    float y2 = lineEnd[1];

    if (isPointInRectangle(lineStart, rectCenter, rectHalfDimensions)) return true;
    if (isPointInRectangle(lineEnd, rectCenter, rectHalfDimensions)) return true;

    if (x1 < rectLeft && x2 < rectLeft) return false; // left
    if (x1 > rectRight && x2 > rectRight) return false; // right
    if (y1 < rectTop && y2 < rectTop) return false; // top
    if (y1 > rectBottom && y2 > rectBottom) return false; // bottom

    // i <3 youtube for this
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

bool Collision::satCollision(const std::vector<std::vector<float>> &A, const std::vector<std::vector<float>> &B) {
    if (A.empty() || B.empty()) {
        return false;
    }

    std::vector<std::vector<float>> axes = getAxes(A);
    std::vector<std::vector<float>> axesB = getAxes(B);
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

bool Collision::AABBCollision(const std::vector<std::vector<float>>& A, const std::vector<std::vector<float>>& B) {
    if (A.empty() || B.empty()) return false;

    float minA_x = A[0][0];
    float minA_y = A[0][1];
    float maxA_x = A[1][0];
    float maxA_y = A[1][1];

    float minB_x = B[0][0];
    float minB_y = B[0][1];
    float maxB_x = B[1][0];
    float maxB_y = B[1][1];

    return (minA_x <= maxB_x && maxA_x >= minB_x && minA_y <= maxB_y && maxA_y >= minB_y);
}



std::vector<std::vector<float>> Collision::getAxes(const std::vector<std::vector<float>> &polygon) {
    std::vector<std::vector<float>> axes;

    for (size_t i = 0; i < polygon.size(); i++) {
        std::vector<float> edge = subtractVectors(polygon[(i + 1) % polygon.size()], polygon[i]);
        std::vector<float> normal = normalize({-edge[1], edge[0]});
        if (magnitude(normal) > 0) { // Ensure the normal is valid
            axes.push_back(normal);
        }
    }

    return axes;
}

void Collision::projectPolygon(const std::vector<std::vector<float>> &polygon, const std::vector<float> &axis, float &minProj, float &maxProj) {
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
