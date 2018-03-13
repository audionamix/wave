#include "wave/header/data_header.h"

#include <cstring>

namespace wave {
DataHeader MakeDataHeader() {
  DataHeader header;
  strncpy(header.sub_chunk_2_id, "data", 4);
  return header;
}
}  // namespace wave
