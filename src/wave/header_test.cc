#include <gtest/gtest.h>

#include "wave/header_list.h"

const std::string gResourcePath(TEST_RESOURCES_PATH);

TEST(Header, List) {
  using namespace wave;
  std::string path(gResourcePath + "/extra-header.wav");

  HeaderList list;
  ASSERT_EQ(list.Init(path), Error::kNoError);
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
