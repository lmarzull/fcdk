//==============================================================================
//  FCDK: Fratal C++ Development Kit
//------------------------------------------------------------------------------
#include <fcdk/shared_library.h>
#include <fcdk/abi.h>

#include <iostream>
#include <dlfcn.h>
#include <utility>



namespace fcdk {


static int to_dlopen(int f) {
  int ret = 0;
  if(f & shared_library::LAZY) ret |= RTLD_LAZY;
  if(f & shared_library::NOW) ret |= RTLD_NOW;
  if(f & shared_library::GLOBAL) ret |= RTLD_GLOBAL;
  if(f & shared_library::LOCAL) ret |= RTLD_LOCAL;
  if(f & shared_library::NODELETE) ret |= RTLD_NODELETE;
  if(f & shared_library::NOLOAD) ret |= RTLD_NOLOAD;
  if(f & shared_library::DEEPBIND) ret |= RTLD_DEEPBIND;
  return ret;
}


//------------------------------------------------------------------------------
shared_library::shared_library(const std::filesystem::path& library_filename,
                               int flags)
  : _handle{nullptr}
  , _flags{flags}
  , _libname{library_filename}
{
  load(_libname, _flags);
}


//------------------------------------------------------------------------------
shared_library::shared_library(shared_library&& moved)
  : _handle(std::exchange(moved._handle, nullptr))
  , _flags(moved._flags)
  , _libname(std::move(moved._libname))
{
}


//------------------------------------------------------------------------------
shared_library&
shared_library::operator=(shared_library&& moved)
{
  if(&moved == this)
    return *this;

  close();
  _handle = std::exchange(moved._handle, nullptr);
  _flags = moved._flags;
  _libname = std::move(moved._libname);
  return *this;
}


//------------------------------------------------------------------------------
shared_library::~shared_library()
{
  try {
    close();
  }
  catch(const std::exception& excp) {
  // avoid exiting destructor with exception
  }
}


//------------------------------------------------------------------------------
bool
shared_library::is_loaded() const
{
  return _handle;
}


//------------------------------------------------------------------------------
void
shared_library::load(const std::filesystem::path& library_filename, int flags)
{
  _handle = dlopen(library_filename.c_str(), to_dlopen(flags));
  if (!is_loaded()) {
    const std::string& error = dlerror();
    RAISE_MSG(system_error, "dlerror: [" << (error.empty() ? "None" : error) << ']');
  }
}


//------------------------------------------------------------------------------
void
shared_library::close()
{
  if(is_loaded() && dlclose(_handle)) {
    RAISE_MSG(system_error, dlerror());
  }
  _handle = nullptr;
}


//------------------------------------------------------------------------------
void*
shared_library::_get_symbol( const char* name ) const
{
  void* sym=nullptr;
  if (is_loaded()) {
    char* error = dlerror();  // Clear any previous error
    sym=dlsym(_handle, name);
    if (!sym && (error=dlerror()))
      RAISE_MSG(system_error, "dlsym error:'" << error << '\'');
  }
  return sym;
}
//------------------------------------------------------------------------------


} // namespace FCDK
