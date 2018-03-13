#ifndef WAVE_HEADER_FMT_HEADER_H_
#define WAVE_HEADER_FMT_HEADER_H_

#include <cstdint>

namespace wave {

struct FMTHeader {
  char sub_chunk_1_id[4];
  uint32_t sub_chunk_1_size;
  uint16_t audio_format;
  uint16_t num_channel;
  uint32_t sample_rate;
  uint32_t byte_rate;
  uint16_t byte_per_block;
  uint16_t bits_per_sample;
};
FMTHeader MakeFMTHeader();

}  // namespace wave

#endif  // WAVE_HEADER_FMT_HEADER_H_
