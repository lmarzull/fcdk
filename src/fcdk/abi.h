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
#ifndef __FCDK_ABI_H__
#define __FCDK_ABI_H__


#include <fcdk/Exception.h>


namespace fcdk::abi {


//==========================================================================
//                             DEMANGLE
//--------------------------------------------------------------------------
/** Demangle the string passed as input parameter.
*
*  The format of the input string is the one return by <code>typeid().
*  name()</code>.
*
*  @param p_type an ASCII string representing the mangled name to demangle.
*
*  @return an ASCII string representing the demangled name.
*
*  @exception  std::invalid_argument
*              The format of the input string is not in a valid mangled
*              name for the compiler.
*
*  @exception  std::bad_alloc
*              When the underlying function does not have enough memory
*              to perform its demangling operation.
*/
//--------------------------------------------------------------------------
std::string	demangle( const std::string& p_type );


} // fcdk::abi


#endif  // End of '#ifndef __GCDK_ABI_H__'
