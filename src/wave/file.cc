#include "wave/file.h"

#include <fstream>
#include <cstring>
#include <limits>
#include <iostream>

#include "wave/headers.h"

namespace wave {

namespace internal {
void NoEncrypt(char* data, size_t size) {}
void NoDecrypt(char* data, size_t size) {}
}  // namespace internal

class File::Impl {
 public:
  Error WriteHeader(uint64_t data_size) {
    if (!ostream.is_open()) {
      return kNotOpen;
    }
    auto original_position = ostream.tellp();
    // Position to beginning of file
    ostream.seekp(0);

    // make header
    auto bits_per_sample = header.fmt.bits_per_sample;
    auto bytes_per_sample = bits_per_sample / 8;
    auto channel_number = header.fmt.num_channel;
    auto sample_rate = header.fmt.sample_rate;

    header.riff.chunk_size =
        sizeof(WAVEHeader) + (data_size * (bits_per_sample / 8)) - 8;
    // fmt header
    header.fmt.byte_per_block = bytes_per_sample * channel_number;
    header.fmt.byte_rate = sample_rate * header.fmt.byte_per_block;
    // data header
    header.data.sub_chunk_2_size = data_size * bytes_per_sample;

    ostream.write(reinterpret_cast<char*>(&header), sizeof(WAVEHeader));
    if (ostream.fail()) {
      return kWriteError;
    }

    // reposition to old position if was > to current position
    if (ostream.tellp() < original_position) {
      ostream.seekp(original_position);
    }
    return kNoError;
  }

  Error ReadHeader() {
    if (!istream.is_open()) {
      return kNotOpen;
    }
    istream.seekg(0, std::ios::end);
    auto file_size = istream.tellg();
    // If not enough data
    if (file_size < sizeof(WAVEHeader)) {
      return kInvalidFormat;
    }
    istream.seekg(0, std::ios::beg);
    istream.read(reinterpret_cast<char*>(&header), sizeof(WAVEHeader));
    if (istream.fail()) {
      return kReadError;
    }

    // check headers ids
    if (std::string(header.riff.chunk_id, 4) != "RIFF") {
      return kInvalidFormat;
    }
    if (std::string(header.riff.format, 4) != "WAVE") {
      return kInvalidFormat;
    }
    if (std::string(header.fmt.sub_chunk_1_id, 4) != "fmt ") {
      return kInvalidFormat;
    }
    if (std::string(header.data.sub_chunk_2_id, 4) != "data") {
      return kInvalidFormat;
    }

    // we only support 8 / 16 / 32  bit per sample
    auto bps = header.fmt.bits_per_sample;
    if (bps != 8 && bps != 16 && bps != 32) {
      return kInvalidFormat;
    }
    return kNoError;
  }

  uint64_t current_sample_index() {
    auto bits_per_sample = header.fmt.bits_per_sample;
    auto bytes_per_sample = bits_per_sample / 8;
    uint64_t data_index = 0;
    if (ostream.is_open()) {
      data_index = static_cast<uint64_t>(ostream.tellp()) - sizeof(WAVEHeader);
    } else if (istream.is_open()) {
      data_index = static_cast<uint64_t>(istream.tellg()) - sizeof(WAVEHeader);
    } else {
      return kNotOpen;
    }
    return data_index / bytes_per_sample;
  }

  uint64_t sample_number() {
    auto bits_per_sample = header.fmt.bits_per_sample;
    auto bytes_per_sample = bits_per_sample / 8;

    auto total_data_size = header.data.sub_chunk_2_size;
    return total_data_size / bytes_per_sample;
  }

  std::ifstream istream;
  std::ofstream ostream;
  WAVEHeader header;
};

File::File() : impl_(new Impl()) { impl_->header = MakeWAVEHeader(); }
File::~File() {
  if (impl_->istream.is_open()) {
    impl_->ostream.flush();
  }
  delete impl_;
}

Error File::Open(const std::string& path, OpenMode mode) {
  if (mode == OpenMode::kOut) {
    impl_->ostream.open(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!impl_->ostream.is_open()) {
      return Error::kFailedToOpen;
    }
    return impl_->WriteHeader(0);
  }

  impl_->istream.open(path.c_str(), std::ios::binary);
  if (!impl_->istream.is_open()) {
    return Error::kFailedToOpen;
  }
  return impl_->ReadHeader();
}

uint16_t File::channel_number() const { return impl_->header.fmt.num_channel; }
void File::set_channel_number(uint16_t channel_number) {
  impl_->header.fmt.num_channel = channel_number;
}

uint32_t File::sample_rate() const { return impl_->header.fmt.sample_rate; }
void File::set_sample_rate(uint32_t sample_rate) {
  impl_->header.fmt.sample_rate = sample_rate;
}

uint16_t File::bits_per_sample() const {
  return impl_->header.fmt.bits_per_sample;
}
void File::set_bits_per_sample(uint16_t bits_per_sample) {
  impl_->header.fmt.bits_per_sample = bits_per_sample;
}

uint64_t File::frame_number() const {
  return impl_->sample_number() / channel_number();
}

Error File::Read(std::vector<float>* output) {
  return Read(internal::NoDecrypt, output);
}

Error File::Read(void (*decrypt)(char*, size_t), std::vector<float>* output) {
  return Read(frame_number(), decrypt, output);
}

Error File::Read(uint64_t frame_number, std::vector<float>* output) {
  return Read(frame_number, internal::NoDecrypt, output);
}

Error File::Read(uint64_t frame_number, void (*decrypt)(char*, size_t),
                 std::vector<float>* output) {
  if (!impl_->istream.is_open()) {
    return kNotOpen;
  }
  auto requested_samples = frame_number * channel_number();

  // check if we have enough data available
  if (impl_->sample_number() <
      requested_samples + impl_->current_sample_index()) {
    return kInvalidFormat;
  }
  // resize output to desired size
  output->resize(requested_samples);

  // read every sample one after another
  for (size_t sample_idx = 0; sample_idx < output->size(); sample_idx++) {
    if (impl_->header.fmt.bits_per_sample == 8) {
      // 8bits case
      int8_t value;
      impl_->istream.read(reinterpret_cast<char*>(&value), sizeof(value));
      decrypt(reinterpret_cast<char*>(&value), sizeof(value) / sizeof(char));
      (*output)[sample_idx] =
          static_cast<float>(value) / std::numeric_limits<int8_t>::max();
    } else if (impl_->header.fmt.bits_per_sample == 16) {
      // 16 bits
      int16_t value;
      impl_->istream.read(reinterpret_cast<char*>(&value), sizeof(value));
      decrypt(reinterpret_cast<char*>(&value), sizeof(value) / sizeof(char));
      (*output)[sample_idx] =
          static_cast<float>(value) / std::numeric_limits<int16_t>::max();
    } else if (impl_->header.fmt.bits_per_sample == 32) {
      // 32bits
      int32_t value;
      impl_->istream.read(reinterpret_cast<char*>(&value), sizeof(value));
      decrypt(reinterpret_cast<char*>(&value), sizeof(value) / sizeof(char));
      (*output)[sample_idx] =
          static_cast<float>(value) / std::numeric_limits<int32_t>::max();
    } else {
      return kInvalidFormat;
    }
  }
  return kNoError;
}

Error File::Write(const std::vector<float>& data) {
  return Write(data, internal::NoEncrypt);
}

Error File::Write(const std::vector<float>& data,
                  void (*encrypt)(char* data, size_t size)) {
  if (!impl_->ostream.is_open()) {
    return kNotOpen;
  }

  auto current_data_size = impl_->current_sample_index();
  auto bits_per_sample = impl_->header.fmt.bits_per_sample;

  // write each samples
  for (const auto sample : data) {
    if (bits_per_sample == 8) {
      // 8bits case
      int8_t value =
          static_cast<int8_t>(sample * std::numeric_limits<int8_t>::max());
      encrypt(reinterpret_cast<char*>(&value), sizeof(value) / sizeof(char));
      impl_->ostream.write(reinterpret_cast<char*>(&value), sizeof(value));
    } else if (bits_per_sample == 16) {
      // 16 bits
      int16_t value =
          static_cast<int16_t>(sample * std::numeric_limits<int16_t>::max());
      encrypt(reinterpret_cast<char*>(&value), sizeof(value) / sizeof(char));
      impl_->ostream.write(reinterpret_cast<char*>(&value), sizeof(value));
    } else if (bits_per_sample == 32) {
      // 32bits
      int32_t value =
          static_cast<int32_t>(sample * std::numeric_limits<int32_t>::max());
      encrypt(reinterpret_cast<char*>(&value), sizeof(value) / sizeof(char));
      impl_->ostream.write(reinterpret_cast<char*>(&value), sizeof(value));
    } else {
      return kInvalidFormat;
    }
  }

  // update header to show the right data size
  impl_->WriteHeader(current_data_size + data.size());

  return kNoError;
}

#if __cplusplus > 199711L

std::error_code make_error_code(Error err) {
  switch (err) {
    case kFailedToOpen:
      return std::make_error_code(std::errc::io_error);
    case kNotOpen:
      return std::make_error_code(std::errc::operation_not_permitted);
    case kInvalidFormat:
      return std::make_error_code(std::errc::executable_format_error);
    case kWriteError:
      return std::make_error_code(std::errc::io_error);
    case kReadError:
      return std::make_error_code(std::errc::io_error);
    default:
      return std::error_code();
  }
}

std::vector<float> File::Read(std::error_code& err) {
  std::vector<float> output;
  auto wave_error = Read(&output);
  err = make_error_code(wave_error);
  return output;
}

std::vector<float> File::Read(uint64_t frame_number, std::error_code& err) {
  std::vector<float> output;
  auto wave_error = Read(frame_number, &output);
  err = make_error_code(wave_error);
  return output;
}

void File::Write(const std::vector<float>& data, std::error_code& err) {
  auto wave_error = Write(data);
  err = make_error_code(wave_error);
}
#endif  // __cplusplus > 199711L

}  // namespace wave
