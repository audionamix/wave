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

enum OpenMode { kIn, kOut };

enum Error {
  kNoError,
  kFailedToOpen,
  kNotOpen,
  kInvalidFormat,
  kWriteError,
  kReadError
};

class File {
 public:
  File();
  ~File();

  /**
   * @brief Open wave file at given path
   */
  Error Open(const std::string& path, OpenMode mode);

  /**
   * @brief Read the entire content of file.
   * @note: File has to be opened in kOut mode of kNotOpen will be returned
   */
  Error Read(std::vector<float>* output);

  /**
   * @brief Read the given number of frames from file.
   * @note: File has to be opened in kOut mode of kNotOpen will be returned.
   * If file is too small, kInvalidFormat is returned
   */
  Error Read(uint64_t frame_number, std::vector<float>* output);

  /**
   * @brief Write the given data
   * @note: File has to be opened in kIn mode of kNotOpen will be returned.
   */
  Error Write(const std::vector<float>& data);

  uint16_t channel_number() const;
  void set_channel_number(uint16_t channel_number);

  uint32_t sample_rate() const;
  void set_sample_rate(uint32_t sample_rate);

  uint16_t bits_per_sample() const;
  void set_bits_per_sample(uint16_t bits_per_sample);

 private:
  class Impl;
  Impl* impl_;
};
}  // namespace wave

#endif  // WAVE_WAVE_FILE_H_
