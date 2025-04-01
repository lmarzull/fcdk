#include "doctest.h"
#include <fcdk/abi.h>

//------------------------------------------------------------------------------
TEST_SUITE("abi") {
  TEST_CASE("demangling type 'int' return int") {
    REQUIRE(fcdk::abi::demangle(typeid(int).name()) == "int");
  }
}
