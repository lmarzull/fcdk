#include <doctest/doctest.h>
#include <test-config.h>
#include <fcdk/plugin.h>
#include <fcdk/exception.h>
#include "plugin/test-plugin.h"
#include <filesystem>


SCENARIO("Loading a plugin") {
  GIVEN("A default constructed plugin") {
    fcdk::plugin<TestPluginInterface> plugin;
    WHEN("Loading a not existing library") {
      THEN("Throw a NotFoundError exception") {
        REQUIRE_THROWS_AS(plugin.load(binary_directory/"libplugin.so"), fcdk::system_error);
      }
    }

    WHEN("Loading a library which is not a plugin") {
      THEN("Throw a fcdk::PluginError") {
        REQUIRE_THROWS_AS(plugin.load(binary_directory/"../src/libfcdk.so"), fcdk::plugin_error);
      }
    }

    WHEN("Loading a valid plugin") {
      THEN("The object is callable") {
        plugin.load(binary_directory/"plugin"/"libplugin.so");
        REQUIRE_EQ(plugin->name(), "TestPluginImplementation");
        REQUIRE_EQ(plugin->vendor(), "fcdk - test-plugin.cc");
        REQUIRE_EQ(plugin->version(), "x.y.z");
      }
    }
  }
}
