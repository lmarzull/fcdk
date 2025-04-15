#include <fcdk/plugin.h>
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


#define API_ID  "b8748e15-5bcd-481d-ae6d-11146831fd93"


class TestPluginInterface :  public fcdk::plugin_object_interface
{
public:
  virtual const std::string& name() const override = 0;
  virtual const std::string& version() const override = 0;
  virtual const std::string& vendor() const override = 0;
  virtual const std::string& plugin_implemented_api_id() const override = 0;

  static const std::string& api_id()
  {
    static const std::string api_id{API_ID};
    return api_id;
  }

  virtual const Object1&  getObject1() = 0;
  virtual Object2*        getObject2() = 0;

  virtual bool            plugin_init_called() const = 0;
  virtual bool            plugin_fini_called() const = 0;

protected:
  virtual ~TestPluginInterface() = default;
};
