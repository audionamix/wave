add_library(wave
  ${src}/wave/file.h
  ${src}/wave/file.cc
)

if (${wave_enable_tests})
  add_executable(wave_tests
    ${src}/wave/file_test.cc
  )
  target_link_libraries(wave_tests
    wave
    gtest
    gtest_main
  )
endif ()
