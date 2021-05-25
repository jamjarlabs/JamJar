#ifndef STANDARD_FILE_TEXTURE_FILE_TEXTURE_RESPONSE_HPP
#define STANDARD_FILE_TEXTURE_FILE_TEXTURE_RESPONSE_HPP

#include "standard/file_texture/file_texture_request.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace JamJar::Standard {

enum FileTextureResponseStatus { SUCCESS, FAILURE };

struct FileTextureResponse {
    FileTextureResponseStatus status;
    std::optional<std::string> errorMessage;
    std::unique_ptr<FileTextureRequest> request;
    std::unique_ptr<std::vector<unsigned char>> data;
    int width;
    int height;
    int nrChannels;
};
}; // namespace JamJar::Standard

#endif
