//==============================================================================
// FCDK  -   Fratal C++ Development Kit
//------------------------------------------------------------------------------
#include <fcdk/abi.h>
#include <fcdk/exception.h>

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
      throw std::bad_alloc();
    else if(status == -2 || status == -3)
      fcdk::throw_error<std::invalid_argument>()
          << '\'' << typeid_name.c_str()
          << "' does not represant a valid mangled name";
  }

	return demangled.get();
}


} // fcdk::abi
