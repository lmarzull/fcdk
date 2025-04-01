#===============================================================================
add_subdirectory(plugin)

add_executable(test-fcdk
  test-exception.cc
  test-shared_library.cc
  test-plugin.cc
  test-abi.cc
  test-logger.cc
  test-basic_uri.cc
  main.cc
)

# conan dependencies
find_package(doctest REQUIRED)
target_include_directories(test-fcdk PRIVATE ${doctest_INCLUDE_DIRS})


# for fcdk library headers
target_link_libraries(test-fcdk PRIVATE FRATAL::fcdk)

# for cmake generated configuration files
target_include_directories(test-fcdk PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
configure_file(test-config.h.in test-config.h)

# for ctest
add_test( NAME    test-fcdk
          COMMAND $<TARGET_FILE:test-fcdk>)

