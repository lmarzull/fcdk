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
#ifndef __FCDK_SHARED_LIBRARY_H__
#define __FCDK_SHARED_LIBRARY_H__


#include <fcdk/Exception.h>
#include <filesystem>


namespace fcdk {


//------------------------------------------------------------------------------
/** A shared library.
 *
 *  This object is not thread safe.
 */
class SharedLibrary final
{
public:
  enum Flags : std::uint8_t {
    LAZY      = 1,
    NOW       = (1 << 1),
    GLOBAL    = (1 << 2),
    LOCAL     = (1 << 3),
    NODELETE  = (1 << 4),
    NOLOAD    = (1 << 5),
    DEEPBIND  = (1 << 6)
  };


  //----------------------------------------------------------------------------
  SharedLibrary();


  //----------------------------------------------------------------------------
  SharedLibrary(std::filesystem::path const& library_filename,
                int  flags = Flags::NOW|Flags::GLOBAL|Flags::DEEPBIND);


  ~SharedLibrary();


  //----------------------------------------------------------------------------
  void  load(const std::filesystem::path& library_filename,
             int f=Flags::NOW|Flags::GLOBAL|Flags::DEEPBIND);


  bool  isLoaded() const;
  void  close();


  //----------------------------------------------------------------------------
  const std::filesystem::path& libraryName() const;
  const std::filesystem::path& systemName() const;


  //--------------------------------------------------------------------------
  template <typename T_symbol_type>
      T_symbol_type
      getSymbol(const char* symbol_name) const;


private:
  void* _handle;

  void* _get_symbol(const char* name) const;
};


//------------------------------------------------------------------------------
template <typename T_symbol_type>
T_symbol_type
SharedLibrary::getSymbol(const char* name) const
{
  return reinterpret_cast<T_symbol_type>(_get_symbol(name));
}

} // namespace FCDK


#endif  // end of #ifdef __FCDK_SHARED_LIBRARY_H__
