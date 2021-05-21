#include "geometry/matrix_4d.hpp"
#include <cmath>

const float PI = 3.14159265358979323846;

JamJar::Matrix4D::Matrix4D() : Matrix4D(std::vector<float>{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}) {}

JamJar::Matrix4D::Matrix4D(const std::vector<float> &data) : data(data) {}

JamJar::Matrix4D *JamJar::Matrix4D::Translate(const Vector2D &translation) {
    this->data[12] = this->data[0] * translation.x + this->data[4] * translation.y + this->data[12];
    this->data[13] = this->data[1] * translation.x + this->data[5] * translation.y + this->data[13];
    this->data[14] = this->data[2] * translation.x + this->data[6] * translation.y + this->data[14];
    this->data[15] = this->data[3] * translation.x + this->data[7] * translation.y + this->data[15];
    return this;
}

JamJar::Matrix4D *JamJar::Matrix4D::Scale(const Vector2D &scale) {
    this->data[0] = this->data[0] * scale.x;
    this->data[1] = this->data[1] * scale.x;
    this->data[2] = this->data[2] * scale.x;
    this->data[3] = this->data[3] * scale.x;
    this->data[4] = this->data[4] * scale.y;
    this->data[5] = this->data[5] * scale.y;
    this->data[6] = this->data[6] * scale.y;
    this->data[7] = this->data[7] * scale.y;
    this->data[8] = 0;
    this->data[9] = 0;
    this->data[10] = 0;
    this->data[11] = 0;
    return this;
}

JamJar::Matrix4D *JamJar::Matrix4D::Rotate(const float &rotation) { return this; }

JamJar::Matrix4D *JamJar::Matrix4D::RotateDeg(const float &rotation) { return this->Rotate(rotation * (PI / 180)); }

JamJar::Matrix4D JamJar::Matrix4D::Ortho(float left, float right, float bottom, float top, float near, float far) {
    auto mat = JamJar::Matrix4D();
    float lr = 1 / (left - right);
    float bt = 1 / (bottom - top);
    float nf = 1 / (near - far);
    mat.data[0] = -2 * lr;
    mat.data[1] = 0;
    mat.data[2] = 0;
    mat.data[3] = 0;
    mat.data[4] = 0;
    mat.data[5] = -2 * bt;
    mat.data[6] = 0;
    mat.data[7] = 0;
    mat.data[8] = 0;
    mat.data[9] = 0;
    mat.data[10] = 2 * nf;
    mat.data[11] = 0;
    mat.data[12] = (left + right) * lr;
    mat.data[13] = (top + bottom) * bt;
    mat.data[14] = (far + near) * nf;
    mat.data[15] = 1;
    return mat;
}
