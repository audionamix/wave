#include "wave/file.h"

#include <fstream>
#include <cstring>
#include <limits>

#include "wave/headers.h"

namespace wave {

class File::Impl {
 public:
  std::string path;
  WAVEHeader header;
};

File::File() : impl_(std::make_shared<Impl>()) {
  impl_->header = MakeWAVEHeader();
}

void File::Open(const std::string& path) {
  impl_->path = path;

  // if file exists and is big enough, read wave header
  std::ifstream stream(impl_->path, std::ios::binary);
  if (!stream.is_open()) {
    return;
  }
  stream.seekg(0, std::ios::end);
  if (stream.tellg() >= sizeof(WAVEHeader)) {
    stream.seekg(0, std::ios::beg);
    stream.read(reinterpret_cast<char*>(&(impl_->header)), sizeof(WAVEHeader));
  }
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

std::vector<float> File::Read(std::error_code& err) {
  auto bits_per_sample = impl_->header.fmt.bits_per_sample;
  auto bytes_per_sample = bits_per_sample / 8;

  auto data_size = impl_->header.data.sub_chunk_2_size;
  auto sample_number = data_size / bytes_per_sample;
  std::vector<float> result(sample_number);

  // open the selected file for reading
  std::ifstream stream(impl_->path, std::ios::binary);
  if (!stream.is_open()) {
    err = std::make_error_code(std::errc::io_error);
    return std::vector<float>();
  }

  // read headers
  stream.read(reinterpret_cast<char*>(&(impl_->header)), sizeof(WAVEHeader));

  // read each sample one after another and convert it to float
  for (size_t sample_idx = 0; sample_idx < result.size(); sample_idx++) {
    if (bits_per_sample == 8) {
      // 8bits case
      int8_t value;
      stream.read(reinterpret_cast<char*>(&value), sizeof(value));
      result[sample_idx] =
          static_cast<float>(value) / std::numeric_limits<int8_t>::max();
    } else if (bits_per_sample == 16) {
      // 16 bits
      int16_t value;
      stream.read(reinterpret_cast<char*>(&value), sizeof(value));
      result[sample_idx] =
          static_cast<float>(value) / std::numeric_limits<int16_t>::max();
    } else if (bits_per_sample == 32) {
      // 32bits
      int32_t value;
      stream.read(reinterpret_cast<char*>(&value), sizeof(value));
      result[sample_idx] =
          static_cast<float>(value) / std::numeric_limits<int32_t>::max();
    } else {
      // any other bit rate isn't supported yet
      err = std::make_error_code(std::errc::not_supported);
      return std::vector<float>();
    }
  }

  return result;
}

void File::Write(const std::vector<float>& data, std::error_code& err) {
  // set by user. Should not be updated
  auto bits_per_sample = impl_->header.fmt.bits_per_sample;
  auto bytes_per_sample = bits_per_sample / 8;
  auto channel_number = impl_->header.fmt.num_channel;
  auto sample_rate = impl_->header.fmt.sample_rate;

  // riff header
  impl_->header.riff.chunk_size =
      sizeof(WAVEHeader) + (data.size() * (bits_per_sample / 8)) - 8;
  // fmt header
  impl_->header.fmt.byte_per_block = bytes_per_sample * channel_number;
  impl_->header.fmt.byte_rate = sample_rate * impl_->header.fmt.byte_per_block;
  // data header
  impl_->header.data.sub_chunk_2_size = data.size() * bytes_per_sample;

  // open file
  std::ofstream stream(impl_->path, std::ios::binary);
  if (!stream.is_open()) {
    err = std::make_error_code(std::errc::io_error);
    return;
  }
  // write header
  stream.write(reinterpret_cast<char*>(&(impl_->header)), sizeof(WAVEHeader));
  // write each samples
  for (const auto sample : data) {
    if (bits_per_sample == 8) {
      // 8bits case
      int8_t value =
          static_cast<int8_t>(sample * std::numeric_limits<int8_t>::max());
      stream.write(reinterpret_cast<char*>(&value), sizeof(value));
    } else if (bits_per_sample == 16) {
      // 16 bits
      int16_t value =
          static_cast<int16_t>(sample * std::numeric_limits<int16_t>::max());
      stream.write(reinterpret_cast<char*>(&value), sizeof(value));
    } else if (bits_per_sample == 32) {
      // 32bits
      int32_t value =
          static_cast<int32_t>(sample * std::numeric_limits<int32_t>::max());
      stream.write(reinterpret_cast<char*>(&value), sizeof(value));
    } else {
      // any other bit rate isn't supported yet
      err = std::make_error_code(std::errc::not_supported);
      return;
    }
  }
  stream.flush();
  stream.close();
}

}  // namespace wave
