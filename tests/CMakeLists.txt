#===============================================================================
CMAKE_MINIMUM_REQUIRED(VERSION 3.23)


ADD_EXECUTABLE(test-fcdk
  test-Exception.cc
)

TARGET_INCLUDE_DIRECTORIES(test-fcdk PRIVATE ${CMAKE_SOURCE_DIR}/src)
