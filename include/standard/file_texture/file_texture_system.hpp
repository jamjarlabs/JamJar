#ifndef STANDARD_FILE_TEXTURE_FILE_TEXTURE_SYSTEM_HPP
#define STANDARD_FILE_TEXTURE_FILE_TEXTURE_SYSTEM_HPP

#include "hash.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "standard/2d/render/renderable.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "system/system.hpp"

namespace JamJar::Standard {
class FileTextureSystem : public System {
  public:
    constexpr static uint32_t MESSAGE_REQUEST_FILE_TEXTURE_LOAD = hash("jamjar_request_file_texture_load");
    constexpr static uint32_t MESSAGE_RESPONSE_FILE_TEXTURE_LOAD = hash("jamjar_response_file_texture_load");
    explicit FileTextureSystem(MessageBus *messageBus);
    void OnMessage(Message *message) override;

  private:
    void loadTexture(std::unique_ptr<FileTextureRequest> request);
};
}; // namespace JamJar::Standard

#endif
