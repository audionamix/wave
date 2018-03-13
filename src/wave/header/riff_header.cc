#include "wave/header/riff_header.h"

#include <cstring>

namespace wave {

RIFFHeader MakeRIFFHeader() {
  RIFFHeader header;
  strncpy(header.chunk_id, "RIFF", 4);
  strncpy(header.format, "WAVE", 4);
  return header;
}
}  // namespace wave
