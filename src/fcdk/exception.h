#ifndef __FCDK_EXCEPTION_H__
#define __FCDK_EXCEPTION_H__


#include <exception>
#include <sstream>
#include <string>


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


#define RAISE( e ) do {\
    throw e("");\
  } while(0)



#define RAISE_MSG( e, msg ) do{\
    std::ostringstream os;\
    os << msg;\
    throw e(os.str());\
  }while(0)


#define RAISE_ERRNO( e, msg ) do{\
    std::ostringstream os;\
    os << msg;\
    char buffer[1024]; char* m = strerror_r(errno, buffer, sizeof(buffer));\
    os << ": " << m;\
    throw e(os.str());\
  }while(0)


namespace fcdk {


DECLARE_EXCEPTION(system_error, std::runtime_error);
DECLARE_EXCEPTION(not_found_error, std::runtime_error);
DECLARE_EXCEPTION(syntax_error, std::runtime_error);
DECLARE_EXCEPTION(bad_value_error, std::runtime_error);


DECLARE_EXCEPTION(bad_state_error, std::logic_error);


}


#endif  // #ifndef __FCDK_EXCEPTION_H__
