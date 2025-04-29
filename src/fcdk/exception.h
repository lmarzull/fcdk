#ifndef __FCDK_EXCEPTION_H__
#define __FCDK_EXCEPTION_H__


#include <stdexcept>
#include <sstream>
#include <functional>
#include <string>
#include <string.h> // strerror_r


#define DECLARE_EXCEPTION(excp, parent)\
class excp  :   public  parent\
{\
public:\
  explicit excp(const std::string& what_arg)\
    : parent(what_arg)\
  {}\
  explicit excp(const char* what_arg)\
    : parent(what_arg)\
  {}\
}


namespace fcdk
{

//------------------------------------------------------------------------------
template <typename T_exception = std::runtime_error>
class throw_error final
{
public:
  throw_error();

  template <typename...T_args>
    throw_error(T_args&&...args);


  throw_error(const throw_error&) = delete;
  throw_error& operator=(const throw_error&) = delete;


  [[noreturn]] ~throw_error() noexcept(false) { throw ctor_(oss_.str()); }


  template <typename T>
    friend throw_error&& operator<<(throw_error&& excp, const T& value)
    {
      excp.oss_ << value;
      return std::move(excp);
    }


private:
  const std::function<T_exception(const std::string&)> ctor_;
  std::ostringstream oss_;
};


//------------------------------------------------------------------------------
template <typename T_exception>
template <typename...T_args>
throw_error<T_exception>::throw_error(T_args&&...args)
  : ctor_(
      std::bind([]<typename...A>(A&&...a) { return T_exception(std::forward<A>(a)...);},
                std::forward<T_args>(args)...)
    )
{
}


//------------------------------------------------------------------------------
template <typename T_exception>
throw_error<T_exception>::throw_error()
  : throw_error(std::placeholders::_1)
{}


} // fcdk
 

//------------------------------------------------------------------------------
#define RAISE( e ) throw_error<e>()


//------------------------------------------------------------------------------
#define RAISE_MSG( e, msg ) throw_error<e>() msg


//------------------------------------------------------------------------------
#define RAISE_ERRNO( e, msg ) do{\
    char buffer[1024]; char* m = strerror_r(errno, buffer, sizeof(buffer));\
    throw_error<e>() msg << ": " << m;\
  }while(0)


namespace fcdk {


DECLARE_EXCEPTION(system_error, std::runtime_error);
DECLARE_EXCEPTION(not_found_error, std::runtime_error);
DECLARE_EXCEPTION(syntax_error, std::runtime_error);
DECLARE_EXCEPTION(bad_value_error, std::runtime_error);


DECLARE_EXCEPTION(bad_state_error, std::logic_error);


}


#endif  // #ifndef __FCDK_EXCEPTION_H__
