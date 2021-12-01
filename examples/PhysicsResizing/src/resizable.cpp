#include "resizable.hpp"

Resizable::Resizable(float min, float max) : Component(Resizable::KEY), min(min), max(max), increasing(true) {}
