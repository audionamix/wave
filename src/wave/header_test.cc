#include <gtest/gtest.h>

#include "wave/header_list.h"

const std::string gResourcePath(TEST_RESOURCES_PATH);

TEST(Header, List) {
  using namespace wave;
  std::string path(gResourcePath + "/extra-header.wav");

  std::shared_ptr<std::ifstream> stream = std::make_shared<std::ifstream>();
  stream->open(path.c_str(), std::ios::binary);

  HeaderList list(stream);
  
  for (auto header : list) {
    std::cout << header.chunk_id() << " " << header.chunk_size() << std::endl;
  }
  
  auto iterator = std::begin(list);
  ASSERT_EQ((*iterator).chunk_id(), "RIFF");
  ASSERT_EQ((*iterator).chunk_size(), 12);
  
  iterator++;
  ASSERT_EQ((*iterator).chunk_id(), "fmt ");
  ASSERT_EQ((*iterator).chunk_size(), 38);
  
  iterator++;
  ASSERT_EQ((*iterator).chunk_id(), "fact");
  ASSERT_EQ((*iterator).chunk_size(), 12);
  
  iterator++;
  ASSERT_EQ((*iterator).chunk_id(), "data");
  ASSERT_EQ((*iterator).chunk_size(), 68228);
}
