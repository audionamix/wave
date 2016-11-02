#include <gtest/gtest.h>

#include "wave/file.h"

const std::string gResourcePath(TEST_RESOURCES_PATH);

TEST(Wave, Read) {
  using namespace wave;
  std::error_code err;
  
  File read_file;
  read_file.Open(gResourcePath + "/test1.wav");
  ASSERT_EQ(read_file.sample_rate(), 44100);
  ASSERT_EQ(read_file.bits_per_sample(), 16);
  ASSERT_EQ(read_file.channel_number(), 2);
  
  auto content = read_file.Read(err);
  ASSERT_FALSE(err);
  ASSERT_EQ(content.size() / read_file.channel_number(),  5.558344671201814 * 44100);
}

TEST(Wave, Write) {
  using namespace wave;
  std::error_code err;
  
  // tested above
  File read_file;
  read_file.Open(gResourcePath + "/test1.wav");
  auto content = read_file.Read(err);
  
  File write_file;
  write_file.Open(gResourcePath + "/output.wav");
  write_file.set_sample_rate(read_file.sample_rate());
  write_file.set_bits_per_sample(read_file.bits_per_sample());
  write_file.set_channel_number(read_file.channel_number());
  write_file.Write(content, err);
  ASSERT_FALSE(err);
  
  
  File re_read_file;
  re_read_file.Open(gResourcePath + "/output.wav");
  auto re_read_content = read_file.Read(err);
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.sample_rate(), re_read_file.sample_rate());
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.bits_per_sample(), re_read_file.bits_per_sample());
  ASSERT_EQ(content, re_read_content);
}
