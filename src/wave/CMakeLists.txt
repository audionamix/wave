add_library(wave
  ${src}/wave/header/data_header.h
  ${src}/wave/header/data_header.cc
  ${src}/wave/header/riff_header.h
  ${src}/wave/header/riff_header.cc
  ${src}/wave/header/fmt_header.h
  ${src}/wave/header/fmt_header.cc
  ${src}/wave/header/wave_header.h
  ${src}/wave/header/wave_header.cc

  ${src}/wave/header.h
  ${src}/wave/header.cc
  ${src}/wave/header_list.h
  ${src}/wave/header_list.cc

  ${src}/wave/error.h
  ${src}/wave/file.h
  ${src}/wave/file.cc
)

# include path
target_include_directories(wave
  INTERFACE
    ${src}
)

# install rules
install(TARGETS wave
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(FILES
  ${src}/wave/file.h
  ${src}/wave/error.h
  DESTINATION include/wave
)

# tests
if (${wave_enable_tests})
  add_executable(wave_tests
    ${src}/wave/file_test.cc
    ${src}/wave/header_test.cc
  )

  add_dependencies(wave_tests
    external_googletest
  )

  if (UNIX AND NOT APPLE)
    target_link_libraries(wave_tests
      gtest
      gtest_main
      wave
      pthread
    )
  elseif(WIN32)
    target_link_libraries(wave_tests
      optimized gtest
      optimized gtest_main
      debug gtestd
      debug gtest_maind
      wave
    )
  else()
    target_link_libraries(wave_tests
      optimized gtest
      optimized gtest_main
      debug gtestd
      debug gtest_maind
      wave
    )
  endif()

  target_include_directories(wave_tests
    PUBLIC
      ${gtest_install_dir}/include
  )
  target_compile_definitions(wave_tests
    PUBLIC -DTEST_RESOURCES_PATH="${test_resource_path}"
  )
endif ()
