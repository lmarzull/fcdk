#include "doctest.h"


#include <fcdk/InterfaceFactory.h>


class Service1
{
};


class Service2
{
};


//------------------------------------------------------------------------------
TEST_SUITE("AbstractFactory") {
  fcdk::InterfaceFactory factory;

  TEST_CASE("Instanciation of unregistered interface throws"){
    REQUIRE_THROWS_AS(factory.getInterface<Service1>(),
                      fcdk::NotFoundError);
  }
}

