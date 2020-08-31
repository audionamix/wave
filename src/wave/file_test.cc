#include <gtest/gtest.h>

#include <cmath>
#include <fstream>
#include <iostream>

#include "wave/file.h"

const std::string gResourcePath(TEST_RESOURCES_PATH);

TEST(Wave, Read) {
  using namespace wave;

  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);

  ASSERT_EQ(read_file.sample_rate(), 44100);
  ASSERT_EQ(read_file.bits_per_sample(), 16);
  ASSERT_EQ(read_file.channel_number(), 2);

  std::vector<float> content;
  read_file.Read(&content);

  ASSERT_EQ(content.size() / read_file.channel_number(),
            5.558344671201814 * 44100);
}

TEST(Wave, ChunkRead) {
  using namespace wave;

  // tested above
  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  std::vector<float> content;
  read_file.Read(&content);

  File chunk_read_file;
  chunk_read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  ASSERT_EQ(content.size() / read_file.channel_number(),
            chunk_read_file.frame_number());

  // Read in two parts
  std::vector<float> p1_content, p2_content;
  const uint64_t kFirstPartSize = 1000;
  auto err = chunk_read_file.Read(kFirstPartSize, &p1_content);
  ASSERT_EQ(err, kNoError);
  err = chunk_read_file.Read(chunk_read_file.frame_number() - kFirstPartSize,
                             &p2_content);
  ASSERT_EQ(err, kNoError);

  // check size
  ASSERT_EQ(content.size(), p1_content.size() + p2_content.size());

  // check if content is the same
  std::vector<float> chunk_content(p1_content.size() + p2_content.size());
  memcpy(chunk_content.data(), p1_content.data(),
         p1_content.size() * sizeof(float));
  memcpy(chunk_content.data() + p1_content.size(), p2_content.data(),
         p2_content.size() * sizeof(float));

  ASSERT_EQ(chunk_content, content);
}

TEST(Wave, Write) {
  using namespace wave;

  // tested above
  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  std::vector<float> content;
  read_file.Read(&content);

  {
    File write_file;
    write_file.Open(gResourcePath + "/output.wav", OpenMode::kOut);
    write_file.set_sample_rate(read_file.sample_rate());
    write_file.set_bits_per_sample(read_file.bits_per_sample());
    write_file.set_channel_number(read_file.channel_number());
    write_file.Write(content);
  }

  // re read
  File re_read_file;
  re_read_file.Open(gResourcePath + "/output.wav", OpenMode::kIn);
  std::vector<float> re_read_content;
  re_read_file.Read(&re_read_content);

  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.sample_rate(), re_read_file.sample_rate());
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.bits_per_sample(), re_read_file.bits_per_sample());
  ASSERT_EQ(content, re_read_content);
}

TEST(Wave, ChunkWrite) {
  using namespace wave;

  // tested above
  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  std::vector<float> content;
  read_file.Read(&content);

  // write per chunk
  {
    File write_file;
    write_file.Open(gResourcePath + "/output.wav", OpenMode::kOut);
    write_file.set_sample_rate(read_file.sample_rate());
    write_file.set_bits_per_sample(read_file.bits_per_sample());
    write_file.set_channel_number(read_file.channel_number());

    const uint64_t kChunkSize = 1000;
    std::vector<float> frames(kChunkSize * read_file.channel_number());
    uint64_t written_samples = 0;
    while (written_samples < content.size()) {
      if (content.size() - written_samples < kChunkSize) {
        frames.resize(content.size() - written_samples);
      }
      memcpy(frames.data(), content.data() + written_samples,
             frames.size() * sizeof(float));
      ASSERT_EQ(write_file.Write(frames), kNoError);
      written_samples += frames.size();
    }
  }

  // re read
  File re_read_file;
  re_read_file.Open(gResourcePath + "/output.wav", OpenMode::kIn);
  std::vector<float> re_read_content;
  re_read_file.Read(&re_read_content);

  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.sample_rate(), re_read_file.sample_rate());
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.bits_per_sample(), re_read_file.bits_per_sample());
  ASSERT_EQ(content, re_read_content);
}

TEST(Wave, Write24bits) {
  using namespace wave;

  // tested above
  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  std::vector<float> content;
  read_file.Read(&content);

  {
    File write_file;
    write_file.Open(gResourcePath + "/output.wav", OpenMode::kOut);
    write_file.set_sample_rate(read_file.sample_rate());
    write_file.set_bits_per_sample(24);
    write_file.set_channel_number(read_file.channel_number());
    write_file.Write(content);
  }

  // re read
  File re_read_file;
  re_read_file.Open(gResourcePath + "/output.wav", OpenMode::kIn);
  std::vector<float> re_read_content;
  re_read_file.Read(&re_read_content);

  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.sample_rate(), re_read_file.sample_rate());
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(re_read_file.bits_per_sample(), 24);
  for (int i = 0; i < read_file.frame_number(); i++) {
    ASSERT_LT(fabs(content[i] - re_read_content[i]), pow(10, -6));
  }
}

#if __cplusplus > 199711L
TEST(Wave, OpenModern) {
  using namespace wave;
  std::error_code err;

  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn, err);

  ASSERT_FALSE(err);

  File incorrect_file;
  incorrect_file.Open("incorrect_path", OpenMode::kIn, err);

  ASSERT_TRUE(err);
}

TEST(Wave, STD__move) {
  using namespace wave;

  // tested above
  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  std::vector<float> content;
  read_file.Read(&content);

  File moved_file = []()->File{
    File chunk_read_file;
    chunk_read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);

    return std::move(chunk_read_file);
  }();

  // Read in two parts
  std::vector<float> p1_content, p2_content;
  const uint64_t kFirstPartSize = 1000;
  auto err = moved_file.Read(kFirstPartSize, &p1_content);
  ASSERT_EQ(err, kNoError);
  err = moved_file.Read(moved_file.frame_number() - kFirstPartSize,
                             &p2_content);
  ASSERT_EQ(err, kNoError);

  // check size
  ASSERT_EQ(content.size(), p1_content.size() + p2_content.size());

  // check if content is the same
  std::vector<float> chunk_content(p1_content.size() + p2_content.size());
  memcpy(chunk_content.data(), p1_content.data(),
         p1_content.size() * sizeof(float));
  memcpy(chunk_content.data() + p1_content.size(), p2_content.data(),
         p2_content.size() * sizeof(float));

  ASSERT_EQ(chunk_content, content);
}

TEST(Wave, ReadModern) {
  using namespace wave;
  std::error_code err;

  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);

  ASSERT_EQ(read_file.sample_rate(), 44100);
  ASSERT_EQ(read_file.bits_per_sample(), 16);
  ASSERT_EQ(read_file.channel_number(), 2);

  auto content = read_file.Read(err);
  ASSERT_FALSE(err);
  ASSERT_EQ(content.size() / read_file.channel_number(),
            5.558344671201814 * 44100);
}

TEST(Wave, WriteModern) {
  using namespace wave;
  std::error_code err;

  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  auto content = read_file.Read(err);

  {
    File write_file;
    write_file.Open(gResourcePath + "/output.wav", OpenMode::kOut);
    write_file.set_sample_rate(read_file.sample_rate());
    write_file.set_bits_per_sample(read_file.bits_per_sample());
    write_file.set_channel_number(read_file.channel_number());
    write_file.Write(content, err);

    ASSERT_FALSE(err);
  }

  // re read
  File re_read_file;
  re_read_file.Open(gResourcePath + "/output.wav", OpenMode::kIn);
  auto re_read_content = re_read_file.Read(err);

  ASSERT_FALSE(err);
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.sample_rate(), re_read_file.sample_rate());
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.bits_per_sample(), re_read_file.bits_per_sample());
  ASSERT_EQ(content, re_read_content);
}

void XOR(char* data, size_t size) {
  for (size_t idx = 0; idx < size; idx++) {
    data[idx] = data[idx] ^ 1;
  }
}

TEST(Wave, ReadWriteXOR) {
  using namespace wave;
  std::error_code err;

  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  auto content = read_file.Read(err);

  // write with xor
  {
    File write_file;
    write_file.Open(gResourcePath + "/output.wav", OpenMode::kOut);
    write_file.set_sample_rate(read_file.sample_rate());
    write_file.set_bits_per_sample(read_file.bits_per_sample());
    write_file.set_channel_number(read_file.channel_number());
    write_file.Write(content, XOR);

    ASSERT_FALSE(err);
  }

  // re read
  File re_read_file;
  re_read_file.Open(gResourcePath + "/output.wav", OpenMode::kIn);
  std::vector<float> re_read_content;
  re_read_file.Read(XOR, &re_read_content);

  ASSERT_FALSE(err);
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.sample_rate(), re_read_file.sample_rate());
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.bits_per_sample(), re_read_file.bits_per_sample());
  ASSERT_EQ(content, re_read_content);
}

TEST(Wave, SeekIn) {
  using namespace wave;

  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  std::vector<float> reference;
  read_file.Read(&reference);

  File read_seek;
  read_seek.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  std::vector<float> p1;
  read_seek.Read(20, &p1);
  ASSERT_EQ(read_seek.Tell(), 20);

  // check p1 is 20 first frames of reference
  for (size_t idx = 0; idx < p1.size(); idx++) {
    ASSERT_EQ(p1[idx], reference[idx]);
  }

  uint64_t first_frame_idx = 10;
  read_seek.Seek(first_frame_idx);
  ASSERT_EQ(read_seek.Tell(), first_frame_idx);
  std::vector<float> p2;
  read_seek.Read(20, &p2);
  ASSERT_EQ(read_seek.Tell(), 30);
  // check that p2 is frame 10 to 30 of reference
  auto first_sample_idx = first_frame_idx * read_file.channel_number();
  for (size_t idx = 0; idx < p2.size(); idx++) {
    ASSERT_EQ(p2[idx], reference[idx + first_sample_idx]);
  }
}

TEST(Wave, SeekOut) {
  using namespace wave;

  File read_file;
  read_file.Open(gResourcePath + "/Untitled3.wav", OpenMode::kIn);
  std::vector<float> p1, p2;
  // read first 20
  read_file.Read(20, &p1);
  ASSERT_EQ(read_file.Tell(), 20);
  // read another 20 frames
  read_file.Read(20, &p2);
  ASSERT_EQ(read_file.Tell(), 40);

  {
    File write_file;
    write_file.Open(gResourcePath + "/output.wav", OpenMode::kOut);
    write_file.set_sample_rate(read_file.sample_rate());
    write_file.set_bits_per_sample(read_file.bits_per_sample());
    write_file.set_channel_number(read_file.channel_number());
    // write
    write_file.Write(p1);
    ASSERT_EQ(write_file.Tell(), 20);
    // seek back 10 frames
    write_file.Seek(10);
    ASSERT_EQ(write_file.Tell(), 10);
    // write another 20
    write_file.Write(p2);
    ASSERT_EQ(write_file.Tell(), 30);
  }

  // re read file
  File re_read_file;
  re_read_file.Open(gResourcePath + "/output.wav", OpenMode::kIn);
  std::vector<float> re_read_content;
  re_read_file.Read(&re_read_content);

  // check content
  for (size_t idx = 0; idx < re_read_content.size(); idx++) {
    if (idx < 10 * read_file.channel_number()) {
      ASSERT_EQ(p1[idx], re_read_content[idx]);
    } else {
      ASSERT_EQ(p2[idx - 10 * read_file.channel_number()],
                re_read_content[idx]);
    }
  }
}

TEST(Wave, ExtraHeaders) {
  using namespace wave;
  File file;
  ASSERT_EQ(file.Open(gResourcePath + "/extra-header.wav", OpenMode::kIn), Error::kNoError);
  
  std::error_code err;
  auto data = file.Read(err);
  ASSERT_FALSE(err);
  File write_file;
  write_file.Open(gResourcePath + "/extra-header-out.wav", OpenMode::kOut);
  write_file.set_channel_number(file.channel_number());
  write_file.set_sample_rate(file.sample_rate());
  write_file.Write(data);
}

#endif  // __cplusplus > 199711L

TEST(Wave, FormatError) {
  using namespace wave;
  File file;
  ASSERT_EQ(file.Open(gResourcePath + "/8kulaw.wav", OpenMode::kIn), Error::kInvalidFormat);
}
