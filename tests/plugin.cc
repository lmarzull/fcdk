#include <fcdk/Plugin.h>
#include "PluginTest.h"

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


class MyPluginInterface : public  ::MyPluginInterface 
{
public:
  MyPluginInterface()
    : _plugin_init_flag{false}
    , _plugin_fini_flag{false}
    , _name{"test_plugin::MyPluginInterface"}
    , _vendor{"fcdk - test-Plugin.cc"}
    , _version{"x.y.z"}
  {}

  virtual ~MyPluginInterface() = default;

  const std::string&  name() const override     { return _name; }
  const std::string&  vendor() const override   { return _vendor; }
  const std::string&  version() const override  { return _version; }

  const Object1&  getObject1() override { return _object1; }
  Object2*        getObject2() override { return &_object2; }

  virtual bool    plugin_init_called() const override { return _plugin_init_flag; }
  virtual bool    plugin_fini_called() const override { return _plugin_fini_flag; }

  bool  _plugin_init_flag;
  bool  _plugin_fini_flag;

private:
  Object1   _object1;
  Object2   _object2;

  const std::string _name;
  const std::string _vendor;
  const std::string _version;
};


} // test_plugin


extern "C" {

test_plugin::MyPluginInterface  plugin_factory;


void  plugin_init()
{

  plugin_factory._plugin_init_flag = true;
}

void  plugin_fini()
{
  plugin_factory._plugin_fini_flag = true;
}


} // extern "C"
