#ifndef WAVE_WAVE_HEADER_H_
#define WAVE_WAVE_HEADER_H_

#include <fstream>
#include <cstdint>

#include "wave/error.h"

namespace wave {

class Header {
 public:
  Error Init(std::ifstream* stream, uint64_t position);
  std::string chunk_id() const;
  uint32_t chunk_size() const;
  uint64_t position() const;

 private:
  std::string id_;
  uint32_t size_;
  uint64_t position_;
};
  
}  // namespace wave

#endif  // WAVE_WAVE_HEADER_H_
