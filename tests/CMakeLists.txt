#===============================================================================
cmake_minimum_required(VERSION 3.23)

add_library(plugin SHARED plugin.cc)
target_include_directories(plugin PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(plugin PRIVATE fcdk)

add_executable(test-fcdk
  test-Exception.cc
  test-SharedLibrary.cc
  test-Plugin.cc
  test-abi.cc
)
target_include_directories(test-fcdk PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(test-fcdk PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(test-fcdk PRIVATE fcdk)

configure_file(test-config.h.in test-config.h)
