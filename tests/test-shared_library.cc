#include <doctest/doctest.h>
#include <test-config.h>
#include <fcdk/exception.h>
#include <fcdk/shared_library.h>


using init = void(*)();


//------------------------------------------------------------------------------
SCENARIO("Default constructor") {
  GIVEN("A shared_library default constructed") {
    fcdk::shared_library  library;
    THEN("The shared library is not loaded") {
      REQUIRE_FALSE(library.is_loaded());
    }
    THEN("Searching symbol throws bad_state_error") {
      REQUIRE_THROWS_AS(library.get_symbol<int*>("any"), fcdk::bad_state_error);
    }
    THEN("Close does not throw") {
      REQUIRE_NOTHROW(library.close());
    }
  }
}


//------------------------------------------------------------------------------
SCENARIO("Loading a shared library") {
  GIVEN("A shared_library default constructed") {
    fcdk::shared_library  library;

    WHEN("Loading a not found library") {
      THEN("Throws a NotFoundError") {
        REQUIRE_THROWS_AS(library.load("not found library"), fcdk::system_error);
        REQUIRE_FALSE(library.is_loaded());
      }
    }

    WHEN("Loading an invalid shared library") {
      THEN("Throws a system_error exception"){
        REQUIRE_THROWS_AS(library.load(source_directory/"test-config.h.in"), fcdk::system_error);
        REQUIRE_FALSE(library.is_loaded());
      }
    }

    WHEN("Loading a valid shared library") {
      THEN("Does not throw") {
        REQUIRE_NOTHROW(library.load(binary_directory/"plugin/libplugin.so"));
        REQUIRE(library.is_loaded());
      }
    }
  }
}


//------------------------------------------------------------------------------
SCENARIO("Getting symbol from a not loaded shared_library") {
  GIVEN("A shared_library default constructed") {
    fcdk::shared_library library;
    WHEN("Searching for a symbol") {
      THEN("Throw a bad_state_error") {
        REQUIRE_THROWS_AS(library.get_symbol<int*>("a_symbol"), fcdk::bad_state_error);
      }
    }
  }
}


//------------------------------------------------------------------------------
SCENARIO("Getting symbol from a loaded shared_library") {
  GIVEN("A shared_library default constructed") {
    fcdk::shared_library library;
    library.load(binary_directory/"plugin/libplugin.so");
    WHEN("Searching for a symbol") {
      THEN("Does not throw") {
        REQUIRE_NOTHROW(library.get_symbol<init>("plugin_init"));
        REQUIRE(library.get_symbol<init>("plugin_init"));
      }
    }
  }
}


//------------------------------------------------------------------------------
SCENARIO("Constructor with library name") {
  GIVEN("A shared_library constructed with a valid shared library") {
    fcdk::shared_library lib(binary_directory/"plugin/libplugin.so");

    THEN("The shared library is loaded") {
      REQUIRE(lib.is_loaded());
    }

    WHEN("Searching for a symbol") {
      THEN("Does not throw") {
        REQUIRE_NOTHROW(lib.get_symbol<init>("plugin_init"));
        REQUIRE(lib.get_symbol<init>("plugin_init"));
      }
    }

    WHEN("Closing the library") {
      THEN("The shared_library is not usable anymore") {
        CHECK_NOTHROW(lib.close());
        REQUIRE_THROWS_AS(lib.get_symbol<init>("plugin_init"), fcdk::bad_state_error);
      }
    }
  }
}


//------------------------------------------------------------------------------
SCENARIO("Move constructor") {
  GIVEN("A valid shared library") {
    fcdk::shared_library source(binary_directory/"plugin/libplugin.so");

    WHEN("Moving the shared library") {
      fcdk::shared_library move(std::move(source));

      THEN("Source library is not loaded") {
        REQUIRE_FALSE(source.is_loaded());
      }
      THEN("Move library is loaded") {
        REQUIRE(move.is_loaded());
        REQUIRE(move.get_symbol<init>("plugin_init"));
      }

      WHEN("Closing the move library") {
        REQUIRE_NOTHROW(move.close());
        THEN("Source library is not loaded") {
          REQUIRE_FALSE(move.is_loaded());
        }
      }
    }
  }
}
