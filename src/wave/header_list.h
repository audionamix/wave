#ifndef WAVE_WAVE_HEADER_LIST_H_
#define WAVE_WAVE_HEADER_LIST_H_

#include <memory>
#include <fstream>

#include "wave/header.h"

namespace wave {

class HeaderList {
 public:
  class Iterator {
   public:
    Iterator(std::shared_ptr<std::ifstream> stream, uint64_t position);
    Iterator operator++();
    Iterator operator++(int);
    Header operator*();
    bool operator==(const Iterator& rhs);
    bool operator!=(const Iterator& rhs);
   private:
    std::shared_ptr<std::ifstream> stream_;
    uint64_t position_;
  };

  HeaderList(std::shared_ptr<std::ifstream> stream_);
  Iterator begin();
  Iterator end();
 private:
  std::shared_ptr<std::ifstream> stream_;
};
}  // namespace wave

#endif  // WAVE_WAVE_HEADER_LIST_H_
