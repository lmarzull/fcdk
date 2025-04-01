//==============================================================================
//  FCDK: Fratal C++ Development Kit
//------------------------------------------------------------------------------
#include <fcdk/exception.h>
#include <fcdk/logger.h>

#include <spdlog/common.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/spdlog.h>
#include <string_view>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <optional>
#include <regex>
#include <cstddef>


namespace fcdk::log {

namespace details {

//------------------------------------------------------------------------------
::spdlog::level::level_enum
to_level(const std::string& str)
{
  if(str=="trace") return ::spdlog::level::trace;
  if(str=="debug") return ::spdlog::level::debug;
  if(str=="info") return ::spdlog::level::info;
  if(str=="warn") return ::spdlog::level::warn;
  if(str=="critical") return ::spdlog::level::critical;
  if(str=="error") return ::spdlog::level::err;
  if(str=="off") return ::spdlog::level::off;
  RAISE_MSG(fcdk::bad_value_error, "Bad level '" << str << '\'');
}



bool
to_bool(const std::string& str)
{
  if((str!="true") && (str!="false"))
    RAISE_MSG(fcdk::bad_value_error, "Wrong boolean value '" << str << '\'');

  return str=="true";
}



//------------------------------------------------------------------------------
struct sink
{
  virtual ~sink() = default;

  std::string                                 name;
  std::optional<std::string>                  pattern;
  std::optional<::spdlog::level::level_enum>  level{::spdlog::level::info};
  bool                                        mt{false};

  virtual void create() 
  {
    if(pattern.has_value()) ::spdlog::get(name)->set_pattern(*pattern);
    if(level.has_value()) ::spdlog::get(name)->set_level(*level);
  }

  virtual void set_property(const std::string& property_name,
                            const std::string& property_value)
  {
    // valid property:
    // - name       [_[:alpha:]]*
    // - pattern    .*
    // - level      see to_level
    // - mt         true|false
    if(property_name=="pattern")  { pattern = property_value; return; }
    if(property_name=="level")    { level = to_level(property_value); return; }
    if(property_name=="mt")       { mt = to_bool(property_value); return; }
    RAISE_MSG(fcdk::bad_value_error, "Invalid property '" << property_name << "' for sink");
  }
};


struct console_sink  : public  sink
{
  bool          color{false};
  std::string   output;


  virtual void create() override
  {
    if(output=="stdout") {
      if(color && mt)   { ::spdlog::stdout_color_mt(name); return; }
      if(color && !mt)  { ::spdlog::stdout_color_st(name); return; }
      if(!color && mt)  { ::spdlog::stdout_logger_mt(name); return; }
      if(!color && !mt) { ::spdlog::stdout_logger_st(name); return; }
    }
    else if(output=="stderr"){
      if(color && mt)   { ::spdlog::stderr_color_mt(name); return; }
      if(color && !mt)  { ::spdlog::stderr_color_st(name); return; }
      if(!color && mt)  { ::spdlog::stderr_logger_mt(name); return; }
      if(!color && !mt) { ::spdlog::stderr_logger_st(name); return; }
    }
    RAISE_MSG(fcdk::bad_value_error,
              "Property 'output' is mandatory for Consol sink '"
              << output << '\'');
  }


  virtual void set_property(const std::string& property_name,
                            const std::string& property_value) override
  {
    // valid property:
    // - color         true|false
    if(property_name=="output")   { output = property_value; return; }
    if(property_name=="color")    { color = to_bool(property_value); return; }
    sink::set_property(property_name, property_value);
  }
};


struct file_sink : public  sink
{
  std::string   filename;
  bool          truncate{false};

  virtual void create() override
  {
    if(mt) ::spdlog::basic_logger_mt(name, filename, truncate);
    else   ::spdlog::basic_logger_st(name, filename, truncate);
  }


  virtual void set_property(const std::string& property_name,
                            const std::string& property_value) override
  {
    // valid property:
    // - color         true|false
    if(property_name=="filename")     { filename = property_value; return; }
    if(property_name=="truncate")    { truncate = to_bool(property_value); return; }
    sink::set_property(property_name, property_value);
  }
};



struct rotating_file_sink : public  file_sink
{
  std::size_t   max_size;
  std::size_t   max_files;
  bool          rotate_on_open{false};

  virtual void create() override
  {
    if(mt)  ::spdlog::rotating_logger_mt(name, filename, max_size, max_files, rotate_on_open);
    else    ::spdlog::rotating_logger_st(name, filename, max_size, max_files, rotate_on_open);
  }

  virtual void set_property(const std::string& property_name,
                            const std::string& property_value) override
  {
    // valid property:
    // - color         true|false
    if(property_name=="max_size")       { max_size = std::stoul(property_value); return; }
    if(property_name=="max_files")      { max_files = std::stoul(property_value); return; }
    if(property_name=="rotate_on_open") { rotate_on_open = to_bool(property_value); return; }
    file_sink::set_property(property_name, property_value);
  }
};


struct daily_file_sink : public  file_sink
{
  // Time at which to create the file
  // 24 hours format; from 0 to 23
  // Midnight is 00:00 and is the begining of a new day
  // Day is running from 00:00 to 23:59
  std::string   hour_min_24;
  std::size_t   max_size;

  virtual void create() override
  {
    if(std::size(hour_min_24)!=5)
      RAISE_MSG(fcdk::bad_value_error, "Invalid hour/minute '" << hour_min_24 << '\'');
    int hour = std::stoi(hour_min_24.substr(0,2));
    int min  = std::stoi(hour_min_24.substr(3,2));
    if(mt) ::spdlog::daily_logger_mt(name, filename, hour, min, truncate, max_size);
    else   ::spdlog::daily_logger_st(name, filename, hour, min, truncate, max_size);
  }


  virtual void set_property(const std::string& property_name,
                            const std::string& property_value) override
  {
    // valid property:
    if(property_name=="hour_min_24")    { hour_min_24 = property_value; return; }
    if(property_name=="max_size")       { max_size = std::stoul(property_value); return; }
    file_sink::set_property(property_name, property_value);
  }
};


std::map<std::string, std::unique_ptr<sink>>  configured_sinks;



std::unique_ptr<sink> create_sink(const std::string& value)
{
  if(value=="Console")          return std::unique_ptr<sink>(new console_sink);
  if(value=="File")             return std::make_unique<file_sink>();
  if(value=="RotatingFile")     return std::make_unique<rotating_file_sink>();
  if(value=="DailyFile")        return std::make_unique<daily_file_sink>();
  RAISE_MSG(fcdk::bad_value_error, '\'' << value << "' is not valid for sink property");
}



//------------------------------------------------------------------------------
std::string_view
trim_begin(const std::string& line)
{
  auto not_space = [](const auto c) { return !std::isspace(c); };
  auto trimed = std::string_view(
                    std::find_if(std::cbegin(line), std::cend(line), not_space),
                    std::cend(line));
  return trimed;
}


constexpr bool
line_is_comment_or_empty(const auto& view)
{
  return view.empty() || view[0] == '#';
}



class false_container
{
public:
  false_container(const std::csub_match& sm)
    : begin_{sm.first, sm.second, r2}
  {}

  std::cregex_iterator  begin() { return begin_; }
  std::cregex_iterator  end() { return end_; }

private:
  std::cregex_iterator  begin_;

  std::cregex_iterator  end_{};

  static const std::regex r2;
};
const std::regex false_container::r2("([^\\.]+)");




//------------------------------------------------------------------------------
auto
check_valid_configuration_line(const std::string_view& line)
{
  static const std::regex r("^spdlog((\\.([^\\.]+))*)\\s*=\\s*(.*)$");
  std::cmatch matches;
  if(!std::regex_match(std::begin(line), std::end(line), matches, r))
    RAISE_MSG(syntax_error, "Invalid syntax");
  return std::make_tuple(std::string_view(matches[1].first, matches[1].second),
                         std::string_view(matches[4].first, matches[4].second));
}


//------------------------------------------------------------------------------
std::tuple<std::string, std::string, std::string>
tokenize(const std::string_view& line)
{
  auto [properties, value] = check_valid_configuration_line(line);

  static const std::regex word_regex("([^\\.]+)");
  // get an iterator to iterate throw .value part of the 'properties' string
  // For example, if line is : spdlog.global.color = true
  // begin iterator will be global (i.e. logger name)
  // itr+1 will be color (i.e. propery name applied to logger name)

  auto itr = std::cregex_iterator(std::begin(properties), std::end(properties), word_regex);
  std::string logger_name = itr->str();
  std::string property_name = (++itr)->str();

  return std::make_tuple(logger_name, property_name, std::string(value));
}


//------------------------------------------------------------------------------
bool
parse_line(const std::string& line)
{
  auto trimed_line = trim_begin(line);
  if(line_is_comment_or_empty(trimed_line))
    return false;
  // line is composed of <property> = <value>
  // spdlog.<logger name|global>[.property]+ = <value>
  auto [logger_name, property, value] = tokenize(trimed_line);
  auto sink_itr = configured_sinks.find(logger_name);
  if(sink_itr==configured_sinks.end()) {
    // There's no sink configured until now, the first property
    // should be 'sink' and the value should be type of sink to create
    if(property!="sink")
      RAISE_MSG(fcdk::bad_value_error, "Property error '" << property << "', expecting 'sink'");
    auto sink = create_sink(value);
    sink->name = logger_name;
    configured_sinks[logger_name] = std::move(sink);
  }
  else {
    sink_itr->second->set_property(property, value);
  }
  return true;
}


//------------------------------------------------------------------------------
void
create_sinks()
{
  for(const auto& sink: configured_sinks) {
    sink.second->create();
    sink.second->sink::create();
  }
  auto default_logger = ::spdlog::get("global");
  if(!default_logger)
    default_logger = ::spdlog::stdout_color_mt("global");

  ::spdlog::set_default_logger(default_logger);
}


//------------------------------------------------------------------------------
void
read_configuration_stream(std::istream& input_stream)
{
  std::size_t nb_line=1;
  bool create = true;
  while(input_stream.good()) {
    std::string line;
    std::getline(input_stream, line);
    if(input_stream.good()) {
      try {
        parse_line(line);
        // Line is of kind 
        // <blank>*# ...            => comment
        // spdlog[.name]+ = <value> => property
      }
      catch ( const std::exception& error)
      {
        std::cerr << "Syntax error at line '" << nb_line
                  << "': [" << line << "]" << std::endl;
        std::cerr << error.what() << std::endl;
        create = false;
      }
      ++nb_line;
    }
  }
  if(create)
    create_sinks();
}

}


//------------------------------------------------------------------------------
void
initialize(const std::filesystem::path& config_file)
{
  ::spdlog::shutdown();
  details::configured_sinks.clear();

  if(!std::filesystem::exists(config_file))
    RAISE_MSG(fcdk::not_found_error, "Logger configuration file not found: "
              << config_file);

  std::ifstream input_stream(config_file.native());
  details::read_configuration_stream(input_stream);
  input_stream.close();
}


} // fcdk
