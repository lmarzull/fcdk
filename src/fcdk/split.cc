#include <fcdk/split.h>


namespace fcdk {


//------------------------------------------------------------------------------
void
split(std::string_view line, std::vector<std::string>& fields, char sep)
{
  std::size_t from = 0;
  std::size_t pipe{};
  while((pipe=line.find(sep, from))!=std::string::npos) {
    fields.push_back(std::string(line.substr(from, pipe-from)));
    from=pipe+1;
  }
  fields.push_back(std::string(line.substr(from, pipe-from)));
}


}   // fcdk
