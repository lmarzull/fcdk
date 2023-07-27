#include "doctest.h"
#include <test-config.h>
#include <fcdk/Plugin.h>
#include "PluginTest.h"
#include <filesystem>


SCENARIO("Failed to load a plugin with an empty library name") {
  GIVEN("A plugin") {
    fcdk::Plugin<MyPluginInterface> plugin;
    WHEN("Loading the plugin with an empty filename") {
      THEN("Throws a fcdk::NotFoundError") {
        CHECK_THROWS_AS(plugin.load(""), fcdk::NotFoundError);
      }
    }
  }
}


SCENARIO("Failed to load a plugin with an invalid shared library file") {
  GIVEN("A plugin") {
    fcdk::Plugin<MyPluginInterface> plugin;
    WHEN("Loading a text file as shared library") {
      std::filesystem::path cc_file(source_directory/"test-SharedLibrary.cc");
      THEN("Throws a fcdk::SystemError") {
        CHECK_THROWS_AS(plugin.load(cc_file), fcdk::SystemError);
      }
    }
  }
}


SCENARIO("Loading a plugin which does not define plugin_definition object") {
  GIVEN("A plugin") {
    fcdk::Plugin<MyPluginInterface> plugin;
    WHEN("Loading a shared object without plugin_definition structure") {
      THEN("Throws a fcd::NotFoundError") {
        CHECK_THROWS_AS(plugin.load(binary_directory/"../src/libfcdk.so"), fcdk::PluginError);
      }
    }
  }
}

SCENARIO("Call a method of the plugin factory object") {
  GIVEN("A plugin") {
    fcdk::Plugin<MyPluginInterface> plugin;
    WHEN("Loading a valid plugin") {
      plugin.load(binary_directory/"libplugin.so");
      THEN("Calling a method from the factory return the correct object") {
        CHECK(plugin->plugin_init_called() == true);
        CHECK(plugin->getObject1().name() == "Object1");
        CHECK(plugin->getObject2()->name() == "Object2");
        CHECK(plugin->name() == "test_plugin::MyPluginInterface");
        CHECK(plugin->version() == "x.y.z");
        CHECK(plugin->vendor() == "fcdk - test-Plugin.cc");
      }
    }
  }
}
