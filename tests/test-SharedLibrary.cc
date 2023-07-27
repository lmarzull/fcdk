#include "doctest.h"
#include <test-config.h>
#include <fcdk/Exception.h>
#include <fcdk/SharedLibrary.h>


SCENARIO("Construction of a shared library with empty filename throws an exception") {
  GIVEN("A Shared library built with empty filename") {
    THEN("Throws a fcdk::NotFoundError") {
      CHECK_THROWS_AS(fcdk::SharedLibrary(""), fcdk::NotFoundError);
    }
  }
}


SCENARIO("Construction of a shared library with filename that is not a shared library throws a SystemError") {
  GIVEN("A Shared library built with a text file") {
    std::filesystem::path cc_file(source_directory/"test-SharedLibrary.cc");
    THEN("Throws a fcdk::SystemError") {
      CHECK_THROWS_AS(fcdk::SharedLibrary{cc_file}, fcdk::SystemError);
    }
  }
}


SCENARIO("Specifying an empty filename when loading a shared library throws an exception") {
  GIVEN("A Shared library built with default constructor") {
    fcdk::SharedLibrary shared_library;
    WHEN("Specifying an empty filename to the load method") {
      THEN("Throws a fcdk::NotFoundError") {
        CHECK_THROWS_AS(shared_library.load(""), fcdk::NotFoundError);
      }
    }
  }
}


SCENARIO("Specifying a filename that is not a valid shared library throws a SystemError exception") {
  GIVEN("A Shared library built with default constructor") {
    fcdk::SharedLibrary shared_library{};
    WHEN("Specifying a text filename to the load method") {
      std::filesystem::path cc_file(source_directory/"test-SharedLibrary.cc");
      THEN("Throws a fcdk::SystemError") {
        CHECK_THROWS_AS(shared_library.load(cc_file), fcdk::SystemError);
      }
      AND_THEN("Shared library is not loaded") {
        CHECK(shared_library.isLoaded() == false);
      }
    }
  }
}
