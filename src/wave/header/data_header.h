#ifndef WAVE_HEADER_DATA_HEADER_H_
#define WAVE_HEADER_DATA_HEADER_H_

#include <cstdint>

namespace wave {

struct DataHeader {
  char sub_chunk_2_id[4];
  uint32_t sub_chunk_2_size;
};
DataHeader MakeDataHeader();

}  // namespace wave

#endif  // WAVE_HEADER_DATA_HEADER_H_
