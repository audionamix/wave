#ifndef WAVE_WAVE_HEADERS_H_
#define WAVE_WAVE_HEADERS_H_

namespace wave {

struct RIFFHeader {
  char chunk_id[4];
  uint32_t chunk_size;
  char format[4];
};
RIFFHeader MakeRIFFHeader() {
  RIFFHeader header;
  strncpy(header.chunk_id, "RIFF", 4);
  strncpy(header.format, "WAVE", 4);
  return header;
}

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

struct DataHeader {
  char sub_chunk_2_id[4];
  uint32_t sub_chunk_2_size;
};
DataHeader MakeDataHeader() {
  DataHeader header;
  strncpy(header.sub_chunk_2_id, "data", 4);
  return header;
}

struct WAVEHeader {
  RIFFHeader riff;
  FMTHeader fmt;
  DataHeader data;
};
WAVEHeader MakeWAVEHeader() {
  WAVEHeader header;
  header.riff = MakeRIFFHeader();
  header.fmt = MakeFMTHeader();
  header.data = MakeDataHeader();
  return header;
}

}  // namespace wave

#endif  // WAVE_WAVE_HEADERS_H_
