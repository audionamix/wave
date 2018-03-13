#ifndef WAVE_WAVE_HEADER_LIST_H_
#define WAVE_WAVE_HEADER_LIST_H_

#include <fstream>

#include "wave/header.h"
#include "wave/error.h"

namespace wave {

class HeaderList {
 public:
  class Iterator {
   public:
    Iterator(std::ifstream* stream, uint64_t position);
    Iterator operator++();
    Iterator operator++(int);
    Header operator*();
    bool operator==(const Iterator& rhs);
    bool operator!=(const Iterator& rhs);
   private:
    std::ifstream* stream_;
    uint64_t position_;
  };

  Error Init(const std::string& path);
  Iterator begin();
  Iterator end();
  
  Header riff();
  Header fmt();
  Header data();
  
 private:
  Header header(const std::string& header_id);
  std::ifstream stream_;
};
}  // namespace wave

#endif  // WAVE_WAVE_HEADER_LIST_H_
