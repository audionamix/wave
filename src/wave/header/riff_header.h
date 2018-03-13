#ifndef WAVE_HEADER_RIFF_HEADER_H_
#define WAVE_HEADER_RIFF_HEADER_H_

#include <cstdint>

namespace wave {

struct RIFFHeader {
  char chunk_id[4];
  uint32_t chunk_size;
  char format[4];
};
RIFFHeader MakeRIFFHeader();

}  // namespace wave

#endif  // WAVE_HEADER_RIFF_HEADER_H_
