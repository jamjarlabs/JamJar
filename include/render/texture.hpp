#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP

#include <cstdint>
#include <vector>

namespace JamJar {

enum TextureFilter { NEAREST, BILINEAR, TRILINEAR };
enum TextureWrap { REPEAT, MIRRORED_REPEAT, CLAMP_TO_EDGE };

struct TextureProperties {
    TextureWrap xWrap = TextureWrap::CLAMP_TO_EDGE;
    TextureWrap yWrap = TextureWrap::CLAMP_TO_EDGE;
    TextureFilter minFilter = TextureFilter::BILINEAR;
    TextureFilter magFilter = TextureFilter::BILINEAR;
    bool generateMipmaps = true;
};

class Texture {
  public:
    explicit Texture(std::uint32_t image, std::vector<float> points);
    explicit Texture(std::uint32_t image);
    std::uint32_t image;
    std::vector<float> points;
};
}; // namespace JamJar

#endif
