//==============================================================================
//  FCDK: Fratal C++ Development Kit
//------------------------------------------------------------------------------
#ifndef __FCDK_PLUGIN_H__
#define __FCDK_PLUGIN_H__


#include <atomic>
#include <fcdk/shared_library.h>
#include <functional>
#include <concepts>
#include <iostream>
#include <type_traits>


namespace fcdk {


DECLARE_EXCEPTION(plugin_error, std::runtime_error); 



//==============================================================================
// Howto write a plugin
// --------------------
//
// The plugin should export a factory object. This object should inherite the
// fcdk::PluginObjectInterface.
//
// 1) Write the implementation of the exported factory object
// 2) The user of the plugin will look for an implementation of this interface
//    by searching for the plugin_factory pointer which should be initialized
//    with an instance of the plugin object
//
//    void  __attribute__((constructor)) plugin_init()
//    {
//      static test_plugin::TestPluginImplementation  plugin_factory_instance;
//      plugin_factory = &plugin_factory_instance;
//    }
//





//==============================================================================
/** The interface each plugin should define
 *
 *  Each plugin should define one such object. The loader of the plugin will
 *  get the instance defining this interface using the Plugin interface below.
 *  The operator '->' should be used to call method of the object from The
 *  plugin.
 *
 */
class plugin_object_interface
{
public:
  virtual const std::string& name() const = 0;
  virtual const std::string& vendor() const = 0;
  virtual const std::string& version() const = 0;
  virtual const std::string& plugin_implemented_api_id() const = 0;

protected:
  virtual ~plugin_object_interface() = default;
};


//------------------------------------------------------------------------------
/** Only object following this concept will be allowed to be created by The
 *  plugin.
 *
 *  The object given as template parameters to plugin object should:
 *  1. Inherite from plugin_object_interface
 *  2. Has a static method called plugin_api_id
 *
 *  The static method "plugin_api_id" should return the API ID of the plugin
 *  implemented api. The one with which the plugin was compiled.
 */
template <typename T>
concept plugin_valid_template =
  requires{
    std::is_base_of<plugin_object_interface, T>::value;
    { T::plugin_api_id() } -> std::same_as<const std::string&>;
  };



//------------------------------------------------------------------------------
/** A plugin.
 *
 *  An object defining an instance of the T_interface object.
 *  Use operator-> to call method of the object created by the plugin.
 */
template <typename T_interface> requires plugin_valid_template<T_interface>
class plugin final
{
public:
  plugin() = default;
  ~plugin();


  //----------------------------------------------------------------------------
  void  load(const std::filesystem::path& library_filename);
  void  close();


  T_interface*        operator->()       { return _interface; }
  const T_interface*  operator->() const { return _interface; }


private:
  shared_library    _shared_library;
  T_interface*      _interface = nullptr;

  void              _load_library(const std::filesystem::path&);
  T_interface*      _get_interface();
};



template <typename T_interface> requires plugin_valid_template<T_interface>
plugin<T_interface>::~plugin()
{
  try {
    close();
  }
  catch(...)
  {}
}



//------------------------------------------------------------------------------
template <typename T_interface> requires plugin_valid_template<T_interface>
void
plugin<T_interface>::load(const std::filesystem::path& library_filename)
{
  _load_library(library_filename);
  _interface = _get_interface();
}



template <typename T_interface> requires plugin_valid_template<T_interface>
void
plugin<T_interface>::_load_library(const std::filesystem::path&  library_filename)
{
  _shared_library.load(library_filename, fcdk::shared_library::LAZY);
}


template <typename T_interface> requires plugin_valid_template<T_interface>
T_interface*
plugin<T_interface>::_get_interface()
{
  // TODO: a refaire
  T_interface* interface{nullptr};
  try {
    interface = *_shared_library.get_symbol<T_interface**>("plugin_factory");
  } catch(const fcdk::system_error& excp ){
    RAISE_MSG(plugin_error, excp.what());
  }

  if(!interface)
    RAISE_MSG(plugin_error, "Invalid plugin: factory not created");

  if(interface->plugin_implemented_api_id() != T_interface::plugin_api_id()) {
    RAISE_MSG(plugin_error, "Invalid plugin: Wrong API Id");
  }
  return interface;
}


template <typename T_interface> requires plugin_valid_template<T_interface>
void
plugin<T_interface>::close()
{
  _shared_library.close();
  _interface = nullptr;
}



} // fcdk


#endif // #ifndef __FCDK_PLUGIN_H__
