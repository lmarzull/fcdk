//==============================================================================
//                          COPYRIGHT INFORMATIONS
//------------------------------------------------------------------------------
/* FCDK  -   Fratal C++ Development Kit
 * Copyright (C) 2007   Laurent Marzullo
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free 
 * Software Foundation; either version 2.1 of the License, or (at your option) 
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
//------------------------------------------------------------------------------
#include <fcdk/abi.h>
#include <fcdk/Exception.h>
#include <stdexcept>
#include <memory>
#include <cstdlib>

// GCC specific include file for api to demangling/mangling name.
#include <cxxabi.h>



namespace fcdk::abi {


//------------------------------------------------------------------------------
std::string
demangle(const std::string& typeid_name)
{
	int     status = 0;
	// the demangled variable must be deallocated with 'free'
  std::unique_ptr<char, decltype(std::free)*>
    demangled(__cxxabiv1::__cxa_demangle(typeid_name.c_str(), 0, 0, &status),
              std::free);

	if(!(status && demangled)) {
    // Launch exception according to 'status'
    //  0   The demangling operation succeeded.
    //  -1  A memory allocation failure occurred.
    //  -2  mangled_name is not a valid name under the C++ ABI mangling rules.
    //  -3  One of the arguments is invalid.
    if(status == -1)
      RAISE(std::bad_alloc);
    else if(status == -2 || status == -3)
      RAISE_MSG(std::invalid_argument,
        '\'' << typeid_name.c_str() << "' does not represant a valid mangled name");
  }

	return std::string(demangled.get());
}



} // fcdk::abi


// --- EOF ---------------------------------------------------------------------
