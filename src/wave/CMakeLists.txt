add_library(wave
  ${src}/wave/headers.h
  ${src}/wave/file.h
  ${src}/wave/file.cc
)

if (${wave_enable_tests})
  add_executable(wave_tests
    ${src}/wave/file_test.cc
  )
  add_dependencies(wave_tests googletest)
  target_link_libraries(wave_tests
    wave
    gtest
    gtest_main
  )
  target_include_directories(wave_tests
    PUBLIC
      ${gtest_install_dir}/include
  )
  target_compile_definitions(wave_tests
    PUBLIC -DTEST_RESOURCES_PATH="${test_resource_path}"
  )
endif ()
