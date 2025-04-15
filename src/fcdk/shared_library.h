//==============================================================================
//  FCDK: Fratal C++ Development Kit
//------------------------------------------------------------------------------
#ifndef __FCDK_SHARED_LIBRARY_H__
#define __FCDK_SHARED_LIBRARY_H__


#include <fcdk/exception.h>
#include <filesystem>


namespace fcdk {


//------------------------------------------------------------------------------
/**  A dlopen wrapper, for loading shared library and searching for symbol
 *
 *  A wrapper for dlopen. dlopen use a void* obscur pointer to point to the
 *  loaded shared library. This void* is reference counted. We then need
 *  to handle copy/move semantic to be as closed as necessary to the real
 *  dlclose/dlopen behavior.
 */
class shared_library final
{
public:
  enum flags : std::uint8_t {
    LAZY      = 1,
    NOW       = (1 << 1),
    GLOBAL    = (1 << 2),
    LOCAL     = (1 << 3),
    NODELETE  = (1 << 4),
    NOLOAD    = (1 << 5),
    DEEPBIND  = (1 << 6)
  };


  //----------------------------------------------------------------------------
  shared_library() = default;


  //----------------------------------------------------------------------------
  /** Load the given library name with specified flags
   *
   *  @library  - A path name to the library to load.
   *
   *  @exception  fcdk::not_found_error - library_filename was not Found
   *              fcdk::system_error    - A problem occurs with dlopen
   */
  shared_library(const std::filesystem::path& library,
                 int  flags = flags::NOW|flags::GLOBAL|flags::DEEPBIND);


  shared_library(const shared_library& copy) = delete;
  shared_library& operator=(const shared_library& copy) = delete;


  //----------------------------------------------------------------------------
  /** Moved constructor
   *
   *  Just move all the attributes to the destination. Nullify the void* handle
   *  of the moved shared object. As the void* is reference counted, we should
   *  not keep two instances with the same void* without doing a dlopen.
   *
   *  @exception  None.
   */
  shared_library(shared_library&& copy);
  shared_library& operator=(shared_library&& copy);


  //----------------------------------------------------------------------------
  ~shared_library();


  /** Load the given library name with specified flags
   *
   *  # Exceptions
   *  *fcdk::not_found_error* library filename was not Found
   *  *fcdk::system_error*    A problem occurs with dlopen
   */
  void  load(const std::filesystem::path& library,
             int f=flags::NOW|flags::GLOBAL|flags::DEEPBIND);


  //----------------------------------------------------------------------------
  /** Close the shared library
   *
   *  @exception  fcdk::system_error - a problem occurs with dlclose
   */
  void  close();


  //--------------------------------------------------------------------------
  template <typename T_symbol_type>
      T_symbol_type
      get_symbol(const char* symbol_name) const;


  /** true if a library was loaded, or false otherwise
   */
  bool  is_loaded() const;

  const std::filesystem::path& library() const;


private:
  void*                   handle_ = nullptr;
  int                     flags_ = 0;
  std::filesystem::path   libname_;

  void* get_symbol_(const char* name) const;
};


//------------------------------------------------------------------------------
template <typename T_symbol_type>
T_symbol_type
shared_library::get_symbol(const char* name) const
{
  if(!is_loaded())
    RAISE_MSG(bad_state_error, "SharedLibrary not loaded!");
  T_symbol_type t = reinterpret_cast<T_symbol_type>(get_symbol_(name));
  return t;
}

} // namespace FCDK


#endif  // end of #ifdef __FCDK_SHARED_LIBRARY_H__
