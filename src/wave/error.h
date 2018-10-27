#ifndef WAVE_WAVE_ERROR_H_
#define WAVE_WAVE_ERROR_H_

namespace wave {

enum Error {
  kNoError = 0,
  kFailedToOpen,
  kNotOpen,
  kInvalidFormat,
  kWriteError,
  kReadError,
  kInvalidSeek
};

}  // namespace wave

#endif  // WAVE_WAVE_ERROR_H_
