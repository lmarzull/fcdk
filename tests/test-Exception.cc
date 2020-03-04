#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <fcdk/Exception.h>


namespace Test {

DECLARE_EXCEPTION(FileNotFoundError, std::runtime_error);

}


//------------------------------------------------------------------------------
TEST_SUITE("Exceptions") {
  TEST_CASE("Exception defined with DECLARE_EXCEPTION are of the parent type"){
    REQUIRE_THROWS_AS(RAISE(Test::FileNotFoundError),
                      std::runtime_error);
  }

  TEST_CASE("Macro RAISE throw the specified exception") {
    REQUIRE_THROWS_AS(RAISE(Test::FileNotFoundError),
                      Test::FileNotFoundError);
  }

  TEST_CASE("Macro RAISE_MSG throw the specified exception with given message"){

    std::string filename("my filename");

    REQUIRE_THROWS_WITH_AS(
        RAISE_MSG(Test::FileNotFoundError, 
                  "File '" << filename << "' not found"),
        "File 'my filename' not found",
        Test::FileNotFoundError);

  }

  TEST_CASE("Macro RAISE_ERRNO throw the specified exception with given message "
            "concatenate to errno system message") {

    std::string filename("my filename");
    errno = EACCES;
    std::string errno_msg(strerror(errno));
    std::string expected_msg("File 'my filename' not found: ");
    expected_msg+=errno_msg;

    REQUIRE_THROWS_WITH_AS(
        RAISE_ERRNO(Test::FileNotFoundError,
                    "File '" << filename << "' not found"),
        expected_msg.c_str(),
        Test::FileNotFoundError);
  }
}

