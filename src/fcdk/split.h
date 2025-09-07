#pragma once


#include <string>
#include <vector>


namespace fcdk {


void split(std::string_view line, std::vector<std::string>& fields, char sep);



} // fcdk
