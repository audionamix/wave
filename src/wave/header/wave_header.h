#ifndef WAVE_HEADER_WAVE_HEADER_H_
#define WAVE_HEADER_WAVE_HEADER_H_

#include "wave/header/data_header.h"
#include "wave/header/fmt_header.h"
#include "wave/header/riff_header.h"

namespace wave {

struct WAVEHeader {
  RIFFHeader riff;
  FMTHeader fmt;
  DataHeader data;
};
WAVEHeader MakeWAVEHeader();

}  // namespace wave

#endif  // WAVE_HEADER_WAVE_HEADER_H_
