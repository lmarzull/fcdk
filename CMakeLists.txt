#===============================================================================
cmake_minimum_required(VERSION 3.23)

#-------------------------------------------------------------------------------
project( fcdk
  DESCRIPTION "Fratal C++ Development Kit"
  LANGUAGES "CXX"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -pedantic)


IF(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  enable_testing()
ENDIF()

add_subdirectory(src)

IF(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(tests)
ENDIF()
