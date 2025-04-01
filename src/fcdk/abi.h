//==============================================================================
// FCDK  -   Fratal C++ Development Kit
//------------------------------------------------------------------------------
#ifndef __FCDK_ABI_H__
#define __FCDK_ABI_H__


#include <fcdk/exception.h>


namespace fcdk::abi {


//==========================================================================
//                             DEMANGLE
//--------------------------------------------------------------------------
/** Demangle the input parameter.
*
*  The format of the input string is the one return by <code>typeid().
*  name()</code>.
*
*  @param mangled_type_name an ASCII string representing the mangled name to
*         demangle.
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
std::string	demangle(const std::string& mangled_type_name);


} // fcdk::abi


#endif  // End of '#ifndef __GCDK_ABI_H__'
