#include <doctest/doctest.h>

#include <fcdk/exception.h>
#include <optional>
#include <string.h>
#include <system_error>


namespace Test {

DECLARE_EXCEPTION(file_not_found_error, std::runtime_error);

}


//------------------------------------------------------------------------------
TEST_SUITE("Exceptions") {
  TEST_CASE("Exception defined with DECLARE_EXCEPTION are of the parent type"){
    REQUIRE_THROWS_AS(fcdk::throw_error<Test::file_not_found_error>(),
                      std::runtime_error);

    REQUIRE_THROWS_AS(fcdk::throw_error<std::out_of_range>(),
                      std::out_of_range);
  }

  TEST_CASE("Exception with no ctor args and no message") {
    REQUIRE_THROWS_AS(fcdk::throw_error<Test::file_not_found_error>(),
                      Test::file_not_found_error);
  }

  TEST_CASE("Exception with no ctor args but with message"){
    std::string filename("my filename");
    REQUIRE_THROWS_WITH_AS(
        fcdk::throw_error<Test::file_not_found_error>() << "File '" << filename << "' not found",
        "File 'my filename' not found",
        Test::file_not_found_error);

  }

  TEST_CASE("Complex CTOR") {
    std::string filename("my filename");
    
    errno = EACCES;
    std::string errno_msg(strerror(errno));
    std::string expected_msg("File 'my filename' not found: ");
    expected_msg+=errno_msg;

    REQUIRE_THROWS_WITH_AS(
        fcdk::throw_error<std::system_error>(errno, std::cref(std::system_category()), std::placeholders::_1)
          << "Et patatra",
        "Et patatra: Permission denied",
        std::system_error);
  }
}

