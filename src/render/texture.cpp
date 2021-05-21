#include "render/texture.hpp"

#include <utility>

const std::vector<float> TEXTURE_POINTS = {1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0};

JamJar::Texture::Texture(std::uint32_t image, std::vector<float> points) : image(image), points(std::move(points)) {}

JamJar::Texture::Texture(std::uint32_t image) : Texture(image, TEXTURE_POINTS) {}
