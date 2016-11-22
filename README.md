## Libwave
A simple cross platform C++ interface for reading WAVE files.

The WAVE file structure taken for reference is:

![alt text](http://i.stack.imgur.com/ITplE.gif)

see [this](http://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file) for more information


## Example
~~~~~~~~~~cpp
#include <iostream>
#include <system_error>

#include "wave/file.h"

int main() {
  // read file's content
  wave::File read_file;
  read_file.Open("/home/gvne/test.wav");
  std::error_code err;
  auto content = read_file.Read(err);
  if (err) {
    std::cout << "Something went wrong" << std::endl;
    return 0;
  }

  // write that same content to another file
  wave::File write_file;
  write_file.Open("/home/gvne/test_write.wav");
  write_file.set_sample_rate(read_file.sample_rate());
  write_file.set_bits_per_sample(read_file.bits_per_sample());
  write_file.set_channel_number(read_file.channel_number());

  write_file.Write(content, err);
  if (err) {
    std::cout << "Something went wrong" << std::endl;
    return 0;
  }

  return 0;
}
~~~~~~~~~~
