#include "geometry/polygon.hpp"

JamJar::Polygon::Polygon(const std::vector<Vector2D> &points) : points(points) {}
JamJar::Polygon::Polygon(const std::vector<float> &points) : points() {
    for (int i = 0; i < points.size(); i += 2) {
        this->points.emplace_back(points[i], points[i + 1]);
    }
}

JamJar::Vector2D JamJar::Polygon::Center() {
    float xSum = 0;
    float ySum = 0;
    for (const auto &point : this->points) {
        xSum += point.x;
        ySum += point.y;
    }
    float numPoints = this->points.size();
    return Vector2D(xSum / (numPoints / 2), ySum / (numPoints / 2));
}

JamJar::Vector2D JamJar::Polygon::FarthestPointInDir(Vector2D direction) {

    if (this->points.size() <= 0) {
        // No points, just return origin
        return JamJar::Vector2D(0, 0);
    }

    float farthestDistance = -1.0f;
    auto farthestPoint = this->points[0];

    for (const auto &point : this->points) {
        float distance = point.x * direction.x + point.y * direction.y;

        if (distance > farthestDistance) {
            farthestPoint = point;
            farthestDistance = distance;
        }
    }

    return farthestPoint;
}

JamJar::Polygon &JamJar::Polygon::Transform(JamJar::Standard::_2D::Transform transform) { return *this; }

bool JamJar::Polygon::PointInside(Vector2D point) { return false; }

std::vector<float> JamJar::Polygon::PointsAsFloats() {
    std::vector<float> floats;
    for (const auto &vector : this->points) {
        floats.push_back(vector.x);
        floats.push_back(vector.y);
    }
    return floats;
}
