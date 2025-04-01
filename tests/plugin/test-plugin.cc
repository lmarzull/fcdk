#include <fcdk/plugin.h>
#include "test-plugin.h"
#include <iostream>


namespace test_plugin {


class Object1 : public ::Object1
{
public:
  std::string  name() const override { return "Object1"; }
};  


class Object2 : public ::Object2
{
public:
  std::string  name() const override { return "Object2"; }
};  


class TestPluginImplementation : public  ::TestPluginInterface 
{
public:
  TestPluginImplementation()
    : _name{"TestPluginImplementation"}
    , _vendor{"fcdk - test-plugin.cc"}
    , _version{"x.y.z"}
    , _plugin_implemented_api_id{API_ID}
  {
  }

  virtual ~TestPluginImplementation() = default;

  const std::string&  name() const override     { return _name; }
  const std::string&  vendor() const override   { return _vendor; }
  const std::string&  version() const override  { return _version; }
  const std::string&  plugin_implemented_api_id() const override { return _plugin_implemented_api_id; }

  const Object1&  getObject1() override { return _object1; }
  Object2*        getObject2() override { return &_object2; }

  virtual bool    plugin_init_called() const override { return _plugin_init_flag; }
  virtual bool    plugin_fini_called() const override { return _plugin_fini_flag; }

  bool        _plugin_init_flag = false;
  bool        _plugin_fini_flag = false;

private:
  Object1   _object1;
  Object2   _object2;

  std::string _name;
  std::string _vendor;
  std::string _version;
  std::string _plugin_implemented_api_id;
};


} // test_plugin



extern "C" {

test_plugin::TestPluginImplementation*   plugin_factory = nullptr;


void  __attribute__((constructor)) plugin_init()
{
  static test_plugin::TestPluginImplementation  plugin_factory_instance;
  plugin_factory = &plugin_factory_instance;
  plugin_factory_instance._plugin_init_flag = true;
}


void  __attribute__((destructor)) plugin_fini()
{
  plugin_factory->_plugin_fini_flag = true;
}


} // extern "C"
