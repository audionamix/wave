cmake_minimum_required(VERSION 3.0)

include(ExternalProject)
project(wave)

# build options
option(wave_enable_tests "Build Unit tests" ON)

# use of c++14
set(CMAKE_CXX_STANDARD 14)

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
endif ()

set(src "${CMAKE_SOURCE_DIR}/src")
include_directories(${src})

add_subdirectory("./src")
