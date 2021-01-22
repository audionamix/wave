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

#include "wave/error.h"

namespace wave {

enum OpenMode { kIn, kOut };

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
   * @note: File has to be opened in kOut mode or kNotOpen will be returned
   */
  Error Read(std::vector<float>* output);

  /**
   * @brief Read the given number of frames from file.
   * @note: File has to be opened in kOut mode or kNotOpen will be returned.
   * If file is too small, kInvalidFormat is returned
   */
  Error Read(uint64_t frame_number, std::vector<float>* output);

  /**
   * @brief Read and decrypt the entire content of file.
   * @note: File has to be opened in kOut mode or kNotOpen will be returned
   */
  Error Read(void (*decrypt)(char* data, size_t size),
             std::vector<float>* output);
  Error Read(uint64_t frame_number, void (*decrypt)(char* data, size_t size),
             std::vector<float>* output);

  /**
   * @brief Write the given data
   * @note: File has to be opened in kIn mode or kNotOpen will be returned.
   * @param clip : if true, hard-clip (force value between -1. and 1.) before writing, 
   * else leave data intact. default to false
   */
  Error Write(const std::vector<float>& data, bool clip = false);

  /**
   * @brief Write and Encrypt using encryption function
   * @note: File has to be opened in kIn mode or kNotOpen will be returned.
   * @param clip : if true, hard-clip (force value between -1. and 1.) before writing, 
   * else leave data intact. default to false
   */
  Error Write(const std::vector<float>& data,
              void (*encrypt)(char* data, size_t size), bool clip = false);
  
  /**
   * Move to the given frame in the file
   */
  Error Seek(uint64_t frame_index);
  
  /**
   * Give the current frame position in the file
   */
  uint64_t Tell() const;

#if __cplusplus >= 201103L
  // C++ 11 available
  File(File&& other);             // Move constructor
  File& operator=(File&& other);  // Move assignement operator
#endif // __cplusplus > 201103L

#if __cplusplus > 199711L
  // Modern C++ interface

  // TODO: add std::function version of Read and Write with encrypted
  std::vector<float> Read(std::error_code& err);
  std::vector<float> Read(uint64_t frame_number, std::error_code& err);
  /**
   * @brief Write the given data
   * @param clip : if true, hard-clip (force value between -1. and 1.) before writing, 
   * else leave data intact. default to false
   */
  void Write(const std::vector<float>& data, std::error_code& err, bool clip = false);
  void Open(const std::string& path, OpenMode mode, std::error_code& err);
#endif  // __cplusplus > 199711L

  uint16_t channel_number() const;
  void set_channel_number(uint16_t channel_number);

  uint32_t sample_rate() const;
  void set_sample_rate(uint32_t sample_rate);

  uint16_t bits_per_sample() const;
  void set_bits_per_sample(uint16_t bits_per_sample);

  uint64_t frame_number() const;
  
 private:
  class Impl;
#if __cplusplus >= 201103L
  std::unique_ptr<Impl> impl_;
#else // prior to c++ 11
  Impl* impl_;
#endif //  __cplusplus <= 201103L
};
}  // namespace wave

#endif  // WAVE_WAVE_FILE_H_
