#===============================================================================
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

FIND_PACKAGE(doctest REQUIRED)

ADD_EXECUTABLE(test-fcdk
  test-Exception.cc
)

TARGET_LINK_LIBRARIES(test-fcdk PRIVATE doctest::doctest)
TARGET_INCLUDE_DIRECTORIES(test-fcdk PRIVATE ${CMAKE_SOURCE_DIR}/src)
