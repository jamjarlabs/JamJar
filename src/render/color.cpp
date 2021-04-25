#include "render/color.hpp"

JamJar::Color::Color() : Color(0.0, 0.0, 0.0, 0.0) {}

JamJar::Color::Color(double red, double green, double blue) : Color(red, green, blue, 1.0) {}

JamJar::Color::Color(double red, double green, double blue, double alpha)
    : m_red(red), m_green(green), m_blue(blue), m_alpha(alpha) {}
