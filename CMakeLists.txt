cmake_minimum_required(VERSION 3.0)

include(ExternalProject)
project(wave)

# build options
option(wave_enable_tests "Build Unit tests" ON)

# if cmake osx deployment target is defined, don't override the cxx standard
if (NOT CMAKE_OSX_DEPLOYMENT_TARGET)
  # use of c++14
  set(CMAKE_CXX_STANDARD 14)
endif()

if (${wave_enable_tests})
  enable_testing()

  set(gtest_install_dir ${CMAKE_BINARY_DIR}/googletest-install)
  ExternalProject_Add(googletest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG "release-1.8.0"
    CMAKE_ARGS
      "-Dgtest_force_shared_crt=ON"
      "-DCMAKE_INSTALL_PREFIX=${gtest_install_dir}"
  )
  link_directories("${gtest_install_dir}/lib")

  # download test resource from https://archive.org/details/test_wav
  set(test_resource_path "${CMAKE_BINARY_DIR}/test_resource")
  file(MAKE_DIRECTORY "${test_resource_path}")
  file(DOWNLOAD "https://archive.org/download/test_wav/Untitled3.wav" "${test_resource_path}/Untitled3.wav")

  # Download wikipedia's different format wave https://en.wikipedia.org/wiki/WAV
  set(test_file_list
    11k16bitpcm.wav
    8k16bitpcm.wav
    11k8bitpcm.wav
    11kulaw.wav
    8k8bitpcm.wav
    8kulaw.wav
    11kadpcm.wav
    8kadpcm.wav
    11kgsm.wav
    8kmp316.wav
    8kgsm.wav
    8ksbc12.wav
    8ktruespeech.wav
    8kmp38.wav
    8kcelp.wav
  )
  foreach(file ${test_file_list})
    message(STATUS "downloading test file ${file}")
    file(DOWNLOAD "http://www.nch.com.au/acm/${file}" "${test_resource_path}/${file}")
  endforeach()
endif ()

set(src "${CMAKE_SOURCE_DIR}/src")
include_directories(${src})

add_subdirectory("./src")
