#include "wave/header/wave_header.h"

namespace wave {
WAVEHeader MakeWAVEHeader() {
  WAVEHeader header;
  header.riff = MakeRIFFHeader();
  header.fmt = MakeFMTHeader();
  header.data = MakeDataHeader();
  return header;
}
}  // namespace wave
