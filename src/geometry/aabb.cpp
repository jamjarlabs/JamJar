#include "geometry/aabb.hpp"

JamJar::AABB::AABB(const Vector2D &dimensions, const Vector2D &center) : dimensions(dimensions), center(center) {}
JamJar::AABB::AABB(const Vector2D &dimensions) : AABB(dimensions, Vector2D(0, 0)) {}

JamJar::Vector2D JamJar::AABB::Center() { return this->center; }

JamJar::Vector2D JamJar::AABB::FarthestPointInDir(Vector2D direction) {
    if (direction.x >= 0) {
        // Right
        auto right = this->center.x + this->dimensions.x / 2;
        if (direction.y >= 0) {
            return Vector2D(right, this->center.y + this->dimensions.y / 2);
        } else {
            return Vector2D(right, this->center.y - this->dimensions.y / 2);
        }
    } else {
        // Left
        auto left = this->center.x - this->dimensions.x / 2;
        if (direction.y >= 0) {
            return Vector2D(left, this->center.y + this->dimensions.y / 2);
        } else {
            return Vector2D(left, this->center.y - this->dimensions.y / 2);
        }
    }
}

JamJar::AABB &JamJar::AABB::Transform(JamJar::Standard::_2D::Transform transform) {
    this->dimensions *= transform.scale;
    this->center += transform.position;
    return *this;
}

bool JamJar::AABB::PointInside(Vector2D point) {
    auto left = this->center.x - this->dimensions.x / 2;
    auto right = this->center.x + this->dimensions.x / 2;
    auto top = this->center.y + this->dimensions.y / 2;
    auto bottom = this->center.y - this->dimensions.y / 2;
    return point.x < right && point.x > left && point.y < top && point.y > bottom;
}
