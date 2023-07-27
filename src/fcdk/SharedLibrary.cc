//==============================================================================
//  FCDK: Fratal C++ Development Kit
//  --------------------------------
//  Copyright (c) 2011 - EURL Fratal
//
//  This library is free software; you can redistribute it and/or modify it
//  under the terms of the GNU Library General Public  License as published by
//  the Free Software Foundation; either version 2 of the License, or (at your
//  option) any later version.
//
//  This library is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
//  License for more details.
//
//  You should have received a copy of the GNU Library General Public License
//  along with this library; if not, write to the Free Software Foundation,
//  Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//------------------------------------------------------------------------------
#include <fcdk/Exception.h>
#include <fcdk/SharedLibrary.h>
#include <fcdk/abi.h>
#include <iostream>
#include <dlfcn.h>
#include <fcdk/abi.h>



namespace fcdk {


static int to_dlopen(int f) {
  int ret = 0;
  if(f & SharedLibrary::LAZY) ret |= RTLD_LAZY;
  if(f & SharedLibrary::NOW) ret |= RTLD_NOW;
  if(f & SharedLibrary::GLOBAL) ret |= RTLD_GLOBAL;
  if(f & SharedLibrary::LOCAL) ret |= RTLD_LOCAL;
  if(f & SharedLibrary::NODELETE) ret |= RTLD_NODELETE;
  if(f & SharedLibrary::NOLOAD) ret |= RTLD_NOLOAD;
  if(f & SharedLibrary::DEEPBIND) ret |= RTLD_DEEPBIND;
  return ret;
}


//------------------------------------------------------------------------------
SharedLibrary::SharedLibrary()
  : _handle(nullptr)
{}



//------------------------------------------------------------------------------
SharedLibrary::SharedLibrary(const std::filesystem::path& library_filename, int flags)
  : _handle(nullptr)
{
  load(library_filename, flags);
}


//------------------------------------------------------------------------------
SharedLibrary::~SharedLibrary()
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
SharedLibrary::isLoaded() const
{
  return _handle;
}


//------------------------------------------------------------------------------
void
SharedLibrary::load(const std::filesystem::path& library_filename, int flags)
{
  if(!std::filesystem::exists(library_filename))
    RAISE_MSG(NotFoundError, library_filename << " not found!");
  _handle = dlopen(library_filename.c_str(), to_dlopen(flags));
  if (!isLoaded())
    RAISE_MSG(SystemError, dlerror());
}


//------------------------------------------------------------------------------
void
SharedLibrary::close()
{
  if(isLoaded() && dlclose(_handle)) {
    RAISE_MSG(SystemError, dlerror());
  }
}


//------------------------------------------------------------------------------
void*
SharedLibrary::_get_symbol( const char* name ) const
{
  void* sym=nullptr;
  if (isLoaded()) {
    sym=dlsym(_handle, name);
    if (!sym)
      RAISE_MSG(SystemError, dlerror());
  }
  return sym;
}
//------------------------------------------------------------------------------


} // namespace FCDK
