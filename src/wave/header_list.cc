#include "wave/header_list.h"

namespace wave {

HeaderList::Iterator::Iterator(std::shared_ptr<std::ifstream> stream,
                               uint64_t position)
    : stream_(stream), position_(position) {}

HeaderList::Iterator HeaderList::Iterator::operator++() {
  Header h(stream_, position_);
  position_ += h.chunk_size();
  return *this;
}

HeaderList::Iterator HeaderList::Iterator::operator++(int) {
  // increment operator is called with an integer argument 0
  // see http://en.cppreference.com/w/cpp/language/operators
  Iterator next = *this;
  operator++();
  return next;
}

Header HeaderList::Iterator::operator*() { return Header(stream_, position_); }

bool HeaderList::Iterator::operator==(const Iterator& rhs) {
  return rhs.stream_.get() == stream_.get() && rhs.position_ == position_;
}
bool HeaderList::Iterator::operator!=(const Iterator& rhs) {
  return !operator==(rhs);
}

HeaderList::HeaderList(std::shared_ptr<std::ifstream> stream)
    : stream_(stream) {}

HeaderList::Iterator HeaderList::begin() {
  return HeaderList::Iterator(stream_, 0);
}

HeaderList::Iterator HeaderList::end() {
  stream_->seekg(0, std::ios::end);
  uint64_t size = stream_->tellg();
  stream_->seekg(0, std::ios::beg);
  return HeaderList::Iterator(stream_, size);
}

}  // namespace wave
