#include "wave/header.h"

#include "wave/header/riff_header.h"

namespace wave {
  Error Header::Init(std::ifstream* stream, uint64_t position) {
    position_ = position;
    if (!stream->is_open()) {
      return Error::kNotOpen;
    }

    // read chunk ID
    const auto chunk_id_size = 4;
    stream->seekg(position_, std::ios::beg);
    char result[chunk_id_size];
    stream->read(result, chunk_id_size * sizeof(char));
    id_ = std::string(result, chunk_id_size);

    // and size
    stream->read(reinterpret_cast<char*>(&size_), sizeof(uint32_t));
    size_ += chunk_id_size * sizeof(char) + sizeof(uint32_t);

    return Error::kNoError;
  }

std::string Header::chunk_id() const {
  return id_;
}

uint32_t Header::chunk_size() const {
  if (chunk_id() == "RIFF") {
    return sizeof(wave::RIFFHeader);
  }
  return size_;
}

uint64_t Header::position() const {
  return position_;
}
};  // namespace wave
