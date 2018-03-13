#include "wave/header/fmt_header.h"

#include <cstring>

namespace wave {
FMTHeader MakeFMTHeader() {
  FMTHeader header;
  strncpy(header.sub_chunk_1_id, "fmt ", 4);
  header.sub_chunk_1_size = 16;
  // default values
  header.audio_format = 1;  // PCM
  header.num_channel = 1;
  header.sample_rate = 44100;
  header.bits_per_sample = 16;
  header.byte_per_block = (header.bits_per_sample * header.num_channel) / 8;
  header.byte_rate = header.byte_per_block * header.sample_rate;
  return header;
}
}  // namespace wave
