#include <doctest/doctest.h>

#include <fcdk/basic_uri.h>
#include <stdexcept>



TEST_CASE("Parsing URI") {
  SUBCASE("Empty string") {
    REQUIRE_THROWS_AS(fcdk::uri var(""), std::format_error);
  }

  SUBCASE("filename: no scheme") {
    REQUIRE_THROWS_AS(fcdk::uri var("/tmp/toto.txt"), std::format_error);
  }

  SUBCASE("filename: with scheme") {
    fcdk::uri var("file:/tmp/toto.txt");
    std::cerr << "path: '" << var.path() << "'\n";
    REQUIRE(var.scheme() == "file");
    REQUIRE(var.path() == "/tmp/toto.txt");
    REQUIRE(var.authority().empty());
    REQUIRE(var.fragment().empty());
  }

  SUBCASE("mail to") {
    fcdk::uri var("mailto:first.last@server.com");
    REQUIRE(var.scheme() == "mailto");
    REQUIRE(var.path() == "first.last@server.com");
    REQUIRE(var.authority().empty());
    REQUIRE(var.fragment().empty());
  }

  SUBCASE("HTTP request") {
    fcdk::uri var("https://localhost.com:8088/foo/bar?param1=val,param2=val2");
    REQUIRE(var.scheme() == "https");
    REQUIRE(var.path() == "/foo/bar");
    REQUIRE(var.authority() == "localhost.com:8088");
    REQUIRE(var.fragment().empty());
  }
}


TEST_CASE("Escape sequence") {
  SUBCASE("Escape Period (%2E)") {
    fcdk::uri var("https://localhost%2Ecom:8088/foo%2Etxt%2Eold/bar?key%5F1=val,param%5F2=val2#fragment%2Eold");

    REQUIRE(var.authority() == "localhost.com:8088");
    REQUIRE(var.path() == "/foo.txt.old/bar");
    REQUIRE(var.fragment() == "fragment.old");
    REQUIRE(var.query() == "key_1=val,param_2=val2");
  }
}
