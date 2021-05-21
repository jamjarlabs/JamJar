#include "render/color.hpp"

JamJar::Color::Color() : Color(0.0, 0.0, 0.0, 0.0) {}

JamJar::Color::Color(double red, double green, double blue) : Color(red, green, blue, 1.0) {}

JamJar::Color::Color(double red, double green, double blue, double alpha)
    : red(red), green(green), blue(blue), alpha(alpha) {}
