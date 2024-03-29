#ifndef GEOMETRY_VECTOR_2D_HPP
#define GEOMETRY_VECTOR_2D_HPP

namespace JamJar {
class Vector2D {
  public:
    float x;
    float y;
    Vector2D();
    Vector2D(const Vector2D &vec);
    explicit Vector2D(float x, float y);
    Vector2D operator+(const Vector2D &other) const;
    void operator+=(const Vector2D &other);
    Vector2D operator-(const Vector2D &other) const;
    void operator-=(const Vector2D &other);
    Vector2D operator*(const Vector2D &other) const;
    void operator*=(const Vector2D &other);
    Vector2D operator*(float scaler) const;
    void operator*=(float scaler);

    [[nodiscard]] float Dot(const Vector2D &other) const;
    [[nodiscard]] float Magnitude() const;
    [[nodiscard]] float Distance(const Vector2D &other) const;

    [[nodiscard]] Vector2D Rotate(const Vector2D &center, float angle) const;
    [[nodiscard]] Vector2D RotateDeg(const Vector2D &center, float angle) const;
    [[nodiscard]] Vector2D Invert() const;
    [[nodiscard]] Vector2D Normalize() const;

    Vector2D *RotateInPlace(const Vector2D &center, float angle);
    Vector2D *RotateDegInPlace(const Vector2D &center, float angle);
    Vector2D *InvertInPlace();
    Vector2D *NormalizeInPlace();
};
}; // namespace JamJar

#endif
