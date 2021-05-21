#ifndef GEOMETRY_VECTOR_2D_HPP
#define GEOMETRY_VECTOR_2D_HPP

namespace JamJar {
class Vector2D {
  public:
    float x;
    float y;
    Vector2D();
    explicit Vector2D(float x, float y);
    Vector2D operator+(const Vector2D &other) const;
    void operator+=(const Vector2D &other);
    Vector2D operator-(const Vector2D &other) const;
    void operator-=(const Vector2D &other);
    Vector2D operator*(const Vector2D &other) const;
    void operator*=(const Vector2D &other);
    Vector2D operator*(float scaler) const;
    void operator*=(float scaler);
};
}; // namespace JamJar

#endif
