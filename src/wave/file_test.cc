#include <gtest/gtest.h>

#include "wave/file.h"

const std::string gResourcePath(TEST_RESOURCES_PATH);

TEST(Wave, Read) {
  using namespace wave;
  
  File read_file;
  read_file.Open(gResourcePath + "/test1.wav");
  ASSERT_EQ(read_file.sample_rate(), 44100);
  ASSERT_EQ(read_file.bits_per_sample(), 16);
  ASSERT_EQ(read_file.channel_number(), 2);
  
#if __cplusplus > 199711L
  std::error_code err;
  auto content = read_file.Read(err);
  ASSERT_FALSE(err);
#else  // __cplusplus > 199711L
  auto content = read_file.Read();
#endif  // __cplusplus > 199711L
  
  ASSERT_EQ(content.size() / read_file.channel_number(),  5.558344671201814 * 44100);
}

TEST(Wave, Write) {
  using namespace wave;
  
  // tested above
  File read_file;
  read_file.Open(gResourcePath + "/test1.wav");
#if __cplusplus > 199711L
  std::error_code err;
  auto content = read_file.Read(err);
  ASSERT_FALSE(err);
#else  // __cplusplus > 199711L
  auto content = read_file.Read();
#endif  // __cplusplus > 199711L
  
  File write_file;
  write_file.Open(gResourcePath + "/output.wav");
  write_file.set_sample_rate(read_file.sample_rate());
  write_file.set_bits_per_sample(read_file.bits_per_sample());
  write_file.set_channel_number(read_file.channel_number());
  
#if __cplusplus > 199711L
  write_file.Write(content, err);
  ASSERT_FALSE(err);
#else  // __cplusplus > 199711L
  write_file.Write(content);
#endif  // __cplusplus > 199711L
  
  
  File re_read_file;
  re_read_file.Open(gResourcePath + "/output.wav");
#if __cplusplus > 199711L
  auto re_read_content = read_file.Read(err);
  ASSERT_FALSE(err);
#else  // __cplusplus > 199711L
  auto re_read_content = read_file.Read();
#endif  // __cplusplus > 199711L
  
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.sample_rate(), re_read_file.sample_rate());
  ASSERT_EQ(read_file.channel_number(), re_read_file.channel_number());
  ASSERT_EQ(read_file.bits_per_sample(), re_read_file.bits_per_sample());
  ASSERT_EQ(content, re_read_content);
}
