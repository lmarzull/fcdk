#include <fcdk/Plugin.h>
#include <string>


class Object1
{
public:
  virtual std::string  name() const = 0;
};  


class Object2
{
public:
  virtual std::string  name() const = 0;
};  


class MyPluginInterface :  public fcdk::PluginFactory
{
public:
  virtual const Object1&  getObject1() = 0;
  virtual Object2*        getObject2() = 0;

  virtual bool            plugin_init_called() const = 0;
  virtual bool            plugin_fini_called() const = 0;

protected:
  virtual ~MyPluginInterface() = default;
};
