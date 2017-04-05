#include <gtest/gtest.h>

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
//
//struct WikipediaFile {
//  int sample_rate;
//  int bits_per_sample;
//  std::string file_name;
//};
//WikipediaFile MakeFileDesc(int sample_rate, int bits_per_sample,
//                           const std::string& file_name) {
//  WikipediaFile f;
//  f.sample_rate = sample_rate;
//  f.bits_per_sample = bits_per_sample;
//  f.file_name = file_name;
//  return f;
//}
//
//TEST(Wave, Wikipedia) {
//  using namespace wave;
//
//  std::vector<WikipediaFile> files_info;
//  files_info.push_back(MakeFileDesc(11025, 16, "11k16bitpcm.wav"));
//  files_info.push_back(MakeFileDesc(8000, 16, "8k16bitpcm.wav"));
//  files_info.push_back(MakeFileDesc(11025, 8, "11k8bitpcm.wav"));
//  files_info.push_back(MakeFileDesc(11025, 8, "11kulaw.wav"));
//  files_info.push_back(MakeFileDesc(8000, 8, "8k8bitpcm.wav"));
//  files_info.push_back(MakeFileDesc(8000, 8, "8kulaw.wav"));
//  files_info.push_back(MakeFileDesc(11025, 4, "11kadpcm.wav"));
//  files_info.push_back(MakeFileDesc(8000, 4, "8kadpcm.wav"));
//  files_info.push_back(MakeFileDesc(11025, 16, "11kgsm.wav"));
//  files_info.push_back(MakeFileDesc(8000, 16, "8kmp316.wav"));
//  files_info.push_back(MakeFileDesc(8000, 16, "8kgsm.wav"));
//  files_info.push_back(MakeFileDesc(8000, 16, "8ksbc12.wav"));
//  files_info.push_back(MakeFileDesc(8000, 16, "8ktruespeech.wav"));
//  files_info.push_back(MakeFileDesc(8000, 16, "8kmp38.wav"));
//  files_info.push_back(MakeFileDesc(8000, 16, "8kcelp.wav"));
//
//  for (const auto& file_info : files_info) {
//    wave::File file;
//#if __cplusplus > 199711L
//    // Some files can't be openned. That's not a problem as long as error is
//    // reported
//    std::error_code err;
//    file.Open(gResourcePath + "/" + file_info.file_name, err);
//    if (err) {
//      continue;
//    }
//    ASSERT_EQ(file.sample_rate(), file_info.sample_rate);
//    auto content = file.Read(err);
//    ASSERT_FALSE(err);
//#else   // __cplusplus > 199711L
//    file.Open(gResourcePath + "/" + file_info.file_name);
//    ASSERT_EQ(file.sample_rate(), file_info.sample_rate);
//#endif  // __cplusplus > 199711L
//  }
//}
