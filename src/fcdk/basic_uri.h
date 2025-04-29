//==============================================================================
//  FCDK: Fratal C++ Development Kit
//------------------------------------------------------------------------------
#ifndef	__FCDK_URI_H__
#define	__FCDK_URI_H__


#include <fcdk/exception.h>
#include <format>
#include <regex>
#include <string>
#include <sstream>


namespace fcdk {

//------------------------------------------------------------------------------
/** Uniform Resource Identifier
 *
 *  This class is based on the RFC3986: http://tools.ietf.org/html/rfc3986#page-13
 *  Example taken from the RFC:
 *  @verbatim
 *    foo://example.com:8042/over/there?name=ferret#nose
 *    \_/   \______________/\_________/ \_________/ \__/
 *     |           |            |            |        |
 *  scheme     authority       path        query   fragment
 *     |   _____________________|__
 *    / \ /                        \
 *    urn:example:animal:ferret:nose
 *  @endverbatim
 *
 *  -# Scheme:  The scheme is mandatory and should follow the following regexp:
 *              ^(([A-Za-z][A-Za-z0-9+.-]+):)
 *  -# authority: authority begin with '//' and until next '/' , '?', '#' or
 *                end of the string
 *  -# path:    begin with '/'
 *  -# query:   begin with '?' list of name=value until '# or end of the
 *              string
 *  -# fragment:  begin with '#' until end of the string
 *
 *  REGEX:
 *  ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?
 *
 *   scheme .... : $2
 *   authority . : $4
 *   path ...... : $5
 *   query ..... : $7
 *   fraglent .. : $9
 *
 *  UNRESERVED CHARACTERS
 *    ALPHA | DIGIT | - | . | _ | ~
 *
 *  ALPHA ..... : %41-%5A
 *                %61-%7A
 *  DIGIT ..... : %30-%39
 *  HYPHEN .... : %2D
 *  PERIOD .... : %2E
 *  UNDERSCORE  : %5F
 *  TILDE ..... : %7E
 */


#define CHAR_RFC3986_URI_REGEX       "^(([A-Za-z0-9+-.]+):)(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?"
#define CHAR16_RFC3986_URI_REGEX    u"^(([A-Za-z0-9+-.]+):)(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?"
#define CHAR32_RFC3986_URI_REGEX    U"^(([A-Za-z0-9+-.]+):)(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?"
//                                     12                  3  4          5       6   7        8 9
#define CHAR_RFC3986_ESCAPE_REGEX       "(%[A-Fa-f0-9]{2})"
#define CHAR16_RFC3986_ESCAPE_REGEX    u"(%[A-Fa-f0-9]{2})"
#define CHAR32_RFC3986_ESCAPE_REGEX    U"(%[A-Fa-f0-9]{2})"
                                       

template <typename CharT> struct RFC3986_regex;


template <> struct RFC3986_regex<char>
{
  static const std::basic_string<char> regex;
  static const std::basic_string<char> escape;
};


template <typename CharT,
          typename Traits = std::char_traits<CharT>,
          typename Allocator = std::allocator<CharT>>
class basic_uri
{
public:
  using traits_type     = Traits;
  using value_type      = CharT;
  using allocator_type  = Allocator;
  using string_type     = std::basic_string<value_type, traits_type, allocator_type>;


  //----------------------------------------------------------------------
  explicit basic_uri(const string_type& uri);

  string_type const& scheme() const;
  string_type const& authority() const;
  string_type const& path() const;
  string_type const& query() const;
  string_type const& fragment() const;

  string_type const& to_str() const { return as_string_; }

private:
  string_type as_string_;
  string_type scheme_;
  string_type authority_;
  string_type path_;
  string_type query_;
  string_type fragment_;
};


//------------------------------------------------------------------------------
template <typename CharT, typename Traits, typename Allocator>
basic_uri<CharT, Traits, Allocator>::basic_uri(const string_type& str)
  : as_string_(str)
{
  using regex_t = std::basic_regex<CharT>;

  regex_t regex(RFC3986_regex<CharT>::regex, std::regex_constants::ECMAScript|std::regex_constants::icase);
  std::match_results<const CharT*>  results;
  if(!std::regex_match(str.c_str(), results, regex)) {
    RAISE_MSG(std::format_error, << str << " is not a valid URI");
  }

  auto extract = [](const std::match_results<const CharT*>& results, std::size_t idx) -> string_type {
    return string_type(results[idx].first, std::distance(results[idx].first, results[idx].second));
  };

  auto unescape = [&extract](const std::match_results<const CharT*>& results, std::size_t idx) -> string_type {
    static regex_t escape(RFC3986_regex<CharT>::escape, std::regex_constants::ECMAScript|std::regex_constants::icase);

    auto escape_begin = std::regex_iterator<const CharT*>(results[idx].first, results[idx].second, escape);
    auto escape_end = std::regex_iterator<const CharT*>();

    string_type unescape_string;
    if(escape_begin==escape_end)
      unescape_string = extract(results, idx);
    else {
      string_type suffix;
      for(std::regex_iterator<const CharT*> i=escape_begin;i!=escape_end;++i) {
        std::match_results<const CharT*> match = *i;
        unescape_string.append(match.prefix());
        unescape_string.push_back(Traits::to_char_type(std::stoi(match.str().c_str()+1, nullptr, 16)));
        suffix = match.suffix();
      }
      unescape_string.append(suffix);
    }

    return unescape_string;
  };

  scheme_ = extract(results, 2);
  authority_ = unescape(results, 4);
  path_ = unescape(results, 5);
  query_ = unescape(results, 7);
  fragment_ = unescape(results, 9);
}


template <typename CharT, typename Traits, typename Allocator>
const std::basic_string<CharT, Traits, Allocator>&
basic_uri<CharT, Traits, Allocator>::scheme() const
{
  return scheme_;
}


template <typename CharT, typename Traits, typename Allocator>
const std::basic_string<CharT, Traits, Allocator>&
basic_uri<CharT, Traits, Allocator>::authority() const
{
  return authority_;
}


template <typename CharT, typename Traits, typename Allocator>
const std::basic_string<CharT, Traits, Allocator>&
basic_uri<CharT, Traits, Allocator>::path() const
{
  return path_;
}


template <typename CharT, typename Traits, typename Allocator>
const std::basic_string<CharT, Traits, Allocator>&
basic_uri<CharT, Traits, Allocator>::fragment() const
{
  return fragment_;
}


template <typename CharT, typename Traits, typename Allocator>
const std::basic_string<CharT, Traits, Allocator>&
basic_uri<CharT, Traits, Allocator>::query() const
{
  return query_;
}


using uri = basic_uri<char>;


} // namespace FCDK


#endif	// #ifndef	__FCDK_URI_H__
