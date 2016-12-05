#ifndef WAVE_WAVE_FILE_H_
#define WAVE_WAVE_FILE_H_

#include <string>
#include <ios>
#include <vector>

#if __cplusplus > 199711L
#include <system_error>
#include <memory>
#endif  // __cplusplus > 199711L

#include <stdint.h>

namespace wave {
class File {
 public:
  File();
  ~File();
  void Open(const std::string& path);
#if __cplusplus > 199711L
  void Open(const std::string& path, std::error_code& err);
#endif  // __cplusplus > 199711L

  uint16_t channel_number() const;
  void set_channel_number(uint16_t channel_number);

  uint32_t sample_rate() const;
  void set_sample_rate(uint32_t sample_rate);

  uint16_t bits_per_sample() const;
  void set_bits_per_sample(uint16_t bits_per_sample);

  // Don't handle error
  std::vector<float> Read();
  void Write(const std::vector<float>& data);

#if __cplusplus > 199711L
  // Read interleaved data
  std::vector<float> Read(std::error_code& err);
  // Write interleaved data
  void Write(const std::vector<float>& data, std::error_code& err);
#endif  // __cplusplus > 199711L

 private:
  int ReadHeader();
  
  class Impl;
  Impl* impl_;
};
}  // namespace wave

#endif  // WAVE_WAVE_FILE_H_
