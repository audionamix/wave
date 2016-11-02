#ifndef WAVE_WAVE_FILE_H_
#define WAVE_WAVE_FILE_H_

#include <string>
#include <system_error>
#include <ios>
#include <vector>
#include <memory>

#include <stdint.h>

namespace wave {
class File {
 public:
  File();
  void Open(const std::string& path);

  uint16_t channel_number() const;
  void set_channel_number(uint16_t channel_number);

  uint32_t sample_rate() const;
  void set_sample_rate(uint32_t sample_rate);

  uint16_t bits_per_sample() const;
  void set_bits_per_sample(uint16_t bits_per_sample);

  // Read interleaved data
  std::vector<float> Read(std::error_code& err);

  // Write interleaved data
  void Write(const std::vector<float>& data, std::error_code& err);

 private:
  class Impl;
  std::shared_ptr<Impl> impl_;
};
}  // namespace wave

#endif  // WAVE_WAVE_FILE_H_
