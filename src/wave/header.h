#ifndef WAVE_WAVE_HEADER_H_
#define WAVE_WAVE_HEADER_H_

#include <memory>
#include <fstream>
#include <cstdint>

#include "wave/header/riff_header.h"
#include "wave/header/fmt_header.h"
#include "wave/header/data_header.h"

namespace wave {

class Header {
 public:
  Header(std::shared_ptr<std::ifstream> stream, uint64_t position);
  std::string chunk_id() const;
  uint32_t chunk_size() const;
  wave::RIFFHeader ToRIFF() const;
  wave::FMTHeader ToFMT() const;
  wave::DataHeader ToData() const;
  uint64_t position() const;

 private:
  std::shared_ptr<std::ifstream> stream_;
  uint64_t position_;
};
  
}  // namespace wave

#endif  // WAVE_WAVE_HEADER_H_
