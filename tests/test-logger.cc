////==============================================================================
//  FCDK: Fratal C++ Development Kit
//------------------------------------------------------------------------------
#include <doctest/doctest.h>
#include <fcdk/exception.h>
#include <fcdk/abi.h>
#include "test-config.h"
#include <fcdk/logger.h>

#include <iostream>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>


namespace fcdk::log::details {
bool parse_line(const std::string&);
std::tuple<std::string, std::string, std::string> tokenize(const std::string_view& v);
}


TEST_SUITE("Property file line parsing") {
  TEST_CASE("Empty line") {
    REQUIRE(fcdk::log::details::parse_line("") == 0);
  }

  TEST_CASE("Only blank line") {
    REQUIRE(fcdk::log::details::parse_line("\t\n\r ") == 0);
  }

  TEST_CASE("Comment line") {
    REQUIRE(fcdk::log::details::parse_line("#") == 0);
  }

  TEST_CASE("Comment line begining with space") {
    REQUIRE(fcdk::log::details::parse_line("\t\v # truc machin") == 0);
  }

  TEST_CASE("truc") {
    fcdk::log::details::tokenize("spdlog.global.level = bidule");
  }
}



SCENARIO("Providing non existing file when initializing logger") {
  GIVEN("A non existing filename") {
    std::filesystem::path config_file(source_directory/"not-existing-file.ini");
    WHEN("Initializing loggers") {
      THEN("Throw a not_found_error") {
        REQUIRE_THROWS_AS(fcdk::log::initialize(config_file), fcdk::not_found_error);
      }
    }
  }
}


SCENARIO("Providing empty file when initializing logger") {
  GIVEN("An empty filename") {
    std::filesystem::path config_file(source_directory/"empty.ini");
    WHEN("Initializing loggers") {
      REQUIRE_NOTHROW(fcdk::log::initialize(config_file));
    }
  }
}


SCENARIO("Initializing console logger") {
  GIVEN("A console logger configuration file") {
    std::filesystem::path config_file(source_directory/"console.ini");
    WHEN("Reading configuration file") {
      REQUIRE_NOTHROW(fcdk::log::initialize(config_file));
      THEN("stdout_logger is defined") {
        auto logger = spdlog::get("stdout_logger");
        REQUIRE(logger.get() != nullptr);
        auto sinks = logger->sinks();
        REQUIRE(dynamic_cast<spdlog::sinks::stdout_sink_st*>(sinks[0].get()) != nullptr);
      }
      THEN("stderr_logger is defined") {
        auto logger = spdlog::get("stderr_logger");
        REQUIRE(logger.get() != nullptr);
      }
    }
  }
}
