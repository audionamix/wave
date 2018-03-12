add_library(wave
  ${src}/wave/headers.h
  ${src}/wave/file.h
  ${src}/wave/file.cc
)

# install rules
install(TARGETS wave
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
install(FILES
  ${src}/wave/file.h
  DESTINATION include/wave
)

# tests
if (${wave_enable_tests})
  include(GoogleTest)
  add_executable(wave_tests
    ${src}/wave/file_test.cc
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

  gtest_add_tests(TARGET wave_tests )

  target_include_directories(wave_tests
    PUBLIC
      ${gtest_install_dir}/include
  )
  target_compile_definitions(wave_tests
    PUBLIC -DTEST_RESOURCES_PATH="${test_resource_path}"
  )
endif ()
