#include "standard/file_texture/file_texture_system.hpp"
#include "message/message.hpp"
#include "message/message_bus.hpp"
#include "message/message_payload.hpp"
#include "standard/file_texture/file_texture_request.hpp"
#include "standard/file_texture/file_texture_response.hpp"
#include "stb_image.h"
#include <iostream>
#include <memory>

JamJar::Standard::FileTextureSystem::FileTextureSystem(MessageBus *messageBus) : System(messageBus) {
    this->messageBus->Subscribe(this, FileTextureSystem::MESSAGE_REQUEST_FILE_TEXTURE_LOAD);
    stbi_set_flip_vertically_on_load(true);
}

void JamJar::Standard::FileTextureSystem::OnMessage(JamJar::Message *message) {
    System::OnMessage(message);
    switch (message->type) {
    case FileTextureSystem::MESSAGE_REQUEST_FILE_TEXTURE_LOAD: {
        auto *loadMessage = static_cast<JamJar::MessagePayload<std::unique_ptr<FileTextureRequest>> *>(message);
        this->loadTexture(std::move(loadMessage->payload));
        break;
    }
    }
}

void JamJar::Standard::FileTextureSystem::loadTexture(std::unique_ptr<FileTextureRequest> request) {
    int width, height, nrChannels;
    auto response = std::unique_ptr<FileTextureResponse>(nullptr);
    unsigned char *raw = stbi_load(request->path.c_str(), &width, &height, &nrChannels, 0);
    if (raw == nullptr) {
        // Error handling
        response =
            std::make_unique<FileTextureResponse>(FileTextureResponse({.status = FileTextureResponseStatus::FAILURE,
                                                                       .errorMessage = stbi_failure_reason(),
                                                                       .request = std::move(request),
                                                                       .data = nullptr,
                                                                       .width = width,
                                                                       .height = height,
                                                                       .nrChannels = nrChannels}));
    } else {
        // Success
        // Set up smart pointer, delete raw data
        auto data = std::make_unique<std::vector<unsigned char>>();
        data->assign(raw, raw + width * height * nrChannels);
        delete[] raw;

        response =
            std::make_unique<FileTextureResponse>(FileTextureResponse({.status = FileTextureResponseStatus::SUCCESS,
                                                                       .request = std::move(request),
                                                                       .data = std::move(data),
                                                                       .width = width,
                                                                       .height = height,
                                                                       .nrChannels = nrChannels}));
    }

    auto msg = std::make_unique<JamJar::MessagePayload<std::unique_ptr<FileTextureResponse>>>(
        JamJar::Standard::FileTextureSystem::MESSAGE_RESPONSE_FILE_TEXTURE_LOAD, std::move(response));
    this->messageBus->Publish(std::move(msg));
}
