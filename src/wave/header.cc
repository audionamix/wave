#include "wave/header.h"

namespace wave {
Header::Header(std::shared_ptr<std::ifstream> stream, uint64_t position) : stream_(stream), position_(position) {}

std::string Header::chunk_id() const {
  stream_->seekg(position_, std::ios::beg);
  auto chunk_id_size = 4;
  char result[chunk_id_size];
  stream_->read(result, chunk_id_size * sizeof(char));
  return std::string(result, chunk_id_size);
}

uint32_t Header::chunk_size() const {
  if (chunk_id() == "RIFF") {
    return sizeof(wave::RIFFHeader);
  }
  stream_->seekg(position_ + 4, std::ios::beg);
  uint32_t result;
  stream_->read(reinterpret_cast<char*>(&result), sizeof(uint32_t));
  return result + 4 + sizeof(uint32_t);  // size of chunk + header id and header size
}

wave::RIFFHeader Header::ToRIFF() const {
  stream_->seekg(position_, std::ios::beg);
  wave::RIFFHeader result;
  stream_->read(reinterpret_cast<char*>(&result), sizeof(wave::RIFFHeader));
  return result;
}

wave::FMTHeader Header::ToFMT() const {
  stream_->seekg(position_, std::ios::beg);
  wave::FMTHeader result;
  stream_->read(reinterpret_cast<char*>(&result), sizeof(wave::FMTHeader));
  return result;
}

wave::DataHeader Header::ToData() const {
  stream_->seekg(position_, std::ios::beg);
  wave::DataHeader result;
  stream_->read(reinterpret_cast<char*>(&result), sizeof(wave::DataHeader));
  return result;
}

uint64_t Header::position() const {
  return position_;
}
};  // namespace wave
