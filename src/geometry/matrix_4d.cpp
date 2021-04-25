#include "geometry/matrix_4d.hpp"

const float PI = 3.14159265358979323846;

JamJar::Matrix4D::Matrix4D() : Matrix4D(std::vector<float>{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}) {}

JamJar::Matrix4D::Matrix4D(const std::vector<float> &data) : m_data(data) {}

JamJar::Matrix4D *JamJar::Matrix4D::Translate(const Vector2D &translation) {
    this->m_data[12] = this->m_data[0] * translation.m_x + this->m_data[4] * translation.m_y + this->m_data[12];
    this->m_data[13] = this->m_data[1] * translation.m_x + this->m_data[5] * translation.m_y + this->m_data[13];
    this->m_data[14] = this->m_data[2] * translation.m_x + this->m_data[6] * translation.m_y + this->m_data[14];
    this->m_data[15] = this->m_data[3] * translation.m_x + this->m_data[7] * translation.m_y + this->m_data[15];
    return this;
}

JamJar::Matrix4D *JamJar::Matrix4D::Scale(const Vector2D &scale) {
    this->m_data[0] = this->m_data[0] * scale.m_x;
    this->m_data[1] = this->m_data[1] * scale.m_x;
    this->m_data[2] = this->m_data[2] * scale.m_x;
    this->m_data[3] = this->m_data[3] * scale.m_x;
    this->m_data[4] = this->m_data[4] * scale.m_y;
    this->m_data[5] = this->m_data[5] * scale.m_y;
    this->m_data[6] = this->m_data[6] * scale.m_y;
    this->m_data[7] = this->m_data[7] * scale.m_y;
    this->m_data[8] = 0;
    this->m_data[9] = 0;
    this->m_data[10] = 0;
    this->m_data[11] = 0;
    return this;
}

JamJar::Matrix4D *JamJar::Matrix4D::Rotate(const float &rotation) {
    // Not yet implemented
    return this;
}

JamJar::Matrix4D *JamJar::Matrix4D::RotateDeg(const float &rotation) {
    // Not yet implemented
    return this->Rotate(rotation * (PI / 180));
}

JamJar::Matrix4D JamJar::Matrix4D::Ortho(float left, float right, float bottom, float top, float near, float far) {
    auto mat = JamJar::Matrix4D();
    float lr = 1 / (left - right);
    float bt = 1 / (bottom - top);
    float nf = 1 / (near - far);
    mat.m_data[0] = -2 * lr;
    mat.m_data[1] = 0;
    mat.m_data[2] = 0;
    mat.m_data[3] = 0;
    mat.m_data[4] = 0;
    mat.m_data[5] = -2 * bt;
    mat.m_data[6] = 0;
    mat.m_data[7] = 0;
    mat.m_data[8] = 0;
    mat.m_data[9] = 0;
    mat.m_data[10] = 2 * nf;
    mat.m_data[11] = 0;
    mat.m_data[12] = (left + right) * lr;
    mat.m_data[13] = (top + bottom) * bt;
    mat.m_data[14] = (far + near) * nf;
    mat.m_data[15] = 1;
    return mat;
}
