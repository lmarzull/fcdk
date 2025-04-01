#include <doctest/doctest.h>

#include <fcdk/exception.h>
#include <string.h>


namespace Test {

DECLARE_EXCEPTION(file_not_found_error, std::runtime_error);

}


//------------------------------------------------------------------------------
TEST_SUITE("Exceptions") {
  TEST_CASE("Exception defined with DECLARE_EXCEPTION are of the parent type"){
    REQUIRE_THROWS_AS(RAISE(Test::file_not_found_error),
                      std::runtime_error);
  }

  TEST_CASE("Macro RAISE throw the specified exception") {
    REQUIRE_THROWS_AS(RAISE(Test::file_not_found_error),
                      Test::file_not_found_error);
  }

  TEST_CASE("Macro RAISE_MSG throw the specified exception with given message"){

    std::string filename("my filename");

    REQUIRE_THROWS_WITH_AS(
        RAISE_MSG(Test::file_not_found_error, 
                  "File '" << filename << "' not found"),
        "File 'my filename' not found",
        Test::file_not_found_error);

  }

  TEST_CASE("Macro RAISE_ERRNO throw the specified exception with given message "
            "concatenate to errno system message") {

    std::string filename("my filename");
    errno = EACCES;
    std::string errno_msg(strerror(errno));
    std::string expected_msg("File 'my filename' not found: ");
    expected_msg+=errno_msg;

    REQUIRE_THROWS_WITH_AS(
        RAISE_ERRNO(Test::file_not_found_error,
                    "File '" << filename << "' not found"),
        expected_msg.c_str(),
        Test::file_not_found_error);
  }
}

