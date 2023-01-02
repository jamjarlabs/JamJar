#ifndef STANDARD_FILE_TEXTURE_FILE_TEXTURE_REQUEST_HPP
#define STANDARD_FILE_TEXTURE_FILE_TEXTURE_REQUEST_HPP

#include "render/texture.hpp"
#include "message/message_bus.hpp"
#include <cstdint>
#include <string>

namespace JamJar::Standard {
struct FileTextureRequest {
    std::uint32_t key;
    std::string path;
    TextureProperties properties = TextureProperties();
};

void LoadTexture(MessageBus *const messageBus, FileTextureRequest *request);
}; // namespace JamJar::Standard

#endif
