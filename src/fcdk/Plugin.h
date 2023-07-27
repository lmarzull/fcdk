#ifndef __FCDK_PLUGIN_H__
#define __FCDK_PLUGIN_H__


#include <atomic>
#include <fcdk/SharedLibrary.h>
#include <functional>
#include <concepts>


namespace fcdk {


DECLARE_EXCEPTION(PluginError, std::runtime_error); 


class PluginFactory
{
public:
  virtual const std::string& name() const = 0;
  virtual const std::string& vendor() const = 0;
  virtual const std::string& version() const = 0;

protected:
  virtual ~PluginFactory() = default;
};

template <typename T>
concept PluginValidFactory = std::is_base_of<PluginFactory, T>::value;


//------------------------------------------------------------------------------
template <PluginValidFactory T_factory>
class Plugin
{
public:
  Plugin() = default;


  //----------------------------------------------------------------------------
  /** Help for load
   *
   *  help for load
   */
  void  load(const std::filesystem::path& library_filename);
  void  close();

  T_factory*        operator->()       { return _factory; }
  const T_factory*  operator->() const { return _factory; }


private:
  SharedLibrary     _shared_library;
  T_factory*        _factory;

  using function_t = void(*)();
  function_t        _plugin_init;
  function_t        _plugin_fini;

  void              _load_library(const std::filesystem::path&);
  void              _call_library_initializer() const;
  T_factory*        _get_factory_instance();
};



//------------------------------------------------------------------------------
template <PluginValidFactory T_factory>
void
Plugin<T_factory>::load(const std::filesystem::path& library_filename)
{
  _load_library(library_filename);
  _call_library_initializer();
  _factory = _get_factory_instance();
  if(!_factory)
    RAISE_MSG(PluginError, "Invalid plugin: factory not created");
}



template <PluginValidFactory T_factory>
void
Plugin<T_factory>::_load_library(const std::filesystem::path&  library_filename)
{
  _shared_library.load(library_filename, 
                       SharedLibrary::Flags::NOW|
                       SharedLibrary::Flags::DEEPBIND|
                       SharedLibrary::Flags::NODELETE|
                       SharedLibrary::Flags::GLOBAL);
  try {
    _plugin_init = _shared_library.getSymbol<function_t>("plugin_init");
    _plugin_fini = _shared_library.getSymbol<function_t>("plugin_fini");
  } catch(const fcdk::SystemError& excp ){
    RAISE_MSG(PluginError, "Plugin init/fini not found");
  }
}


template <PluginValidFactory T_factory>
void
Plugin<T_factory>::_call_library_initializer() const
{
  _plugin_init();
}


template <PluginValidFactory T_factory>
T_factory*
Plugin<T_factory>::_get_factory_instance()
{
  T_factory* factory = nullptr;
  try {
    factory = _shared_library.getSymbol<T_factory*>("plugin_factory");
  } catch(const fcdk::SystemError& excp ){
    RAISE_MSG(PluginError, "Plugin initializer not found");
  }
  return factory;
}


template <PluginValidFactory T_factory>
void
Plugin<T_factory>::close()
{
  _plugin_fini();
  _shared_library.close();
}



} // fcdk


#endif // #ifndef __FCDK_PLUGIN_H__
