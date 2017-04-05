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
//
// struct WikipediaFile {
//  int sample_rate;
//  int bits_per_sample;
//  std::string file_name;
//};
// WikipediaFile MakeFileDesc(int sample_rate, int bits_per_sample,
//                           const std::string& file_name) {
//  WikipediaFile f;
//  f.sample_rate = sample_rate;
//  f.bits_per_sample = bits_per_sample;
//  f.file_name = file_name;
//  return f;
//}
//
// TEST(Wave, Wikipedia) {
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
