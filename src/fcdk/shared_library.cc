//==============================================================================
//  FCDK: Fratal C++ Development Kit
//------------------------------------------------------------------------------
#include <fcdk/shared_library.h>
#include <fcdk/abi.h>

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
  : handle_{nullptr}
  , flags_{flags}
  , libname_{library_filename}
{
  load(libname_, flags_);
}


//------------------------------------------------------------------------------
shared_library::shared_library(shared_library&& moved)
  : handle_(std::exchange(moved.handle_, nullptr))
  , flags_(moved.flags_)
  , libname_(std::move(moved.libname_))
{
}


//------------------------------------------------------------------------------
shared_library&
shared_library::operator=(shared_library&& moved)
{
  if(&moved == this)
    return *this;

  close();
  handle_ = std::exchange(moved.handle_, nullptr);
  flags_ = moved.flags_;
  libname_ = std::move(moved.libname_);
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
  return handle_;
}


//------------------------------------------------------------------------------
void
shared_library::load(const std::filesystem::path& library_filename, int flags)
{
  handle_ = dlopen(library_filename.c_str(), to_dlopen(flags));
  if (!is_loaded()) {
    const std::string& error = dlerror();
    RAISE_ERRNO(system_error, << "dlerror: [" << (error.empty() ? "None" : error) << ']');
  }
}


//------------------------------------------------------------------------------
void
shared_library::close()
{
  if(is_loaded() && dlclose(handle_)) {
    RAISE_MSG(system_error, << dlerror());
  }
  handle_ = nullptr;
}


//------------------------------------------------------------------------------
void*
shared_library::get_symbol_( const char* name ) const
{
  void* sym=nullptr;
  if (is_loaded()) {
    char* error = dlerror();  // Clear any previous error
    sym=dlsym(handle_, name);
    if (!sym && (error=dlerror()))
      RAISE_MSG(system_error, << "dlsym error:'" << error << '\'');
  }
  return sym;
}
//------------------------------------------------------------------------------


} // namespace FCDK
