#ifndef UTIL_STRING_UTILS_HPP
#define UTIL_STRING_UTILS_HPP

#include <cstring>
#include <string>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>

#include "util/string_constants.hpp"

namespace util
{
  /**
   * Splits `s` using `delims` as separator and fills the container `ret` with the parts.
   * An empty string results in an empty container `ret`.
   * @param s input string to split
   * @param delims any character here acts as a separator
   * @param ret return by reference the container filled with the string split.
   */
  template<typename C>
  void string_split(std::string const & s, char const * delims, C & ret)
  {
      C output;

      if (s.size() > 0) {
          char const* p = s.c_str();
          char const* q = strpbrk(p+1, delims);

          // Insert first to last-1 elements
          for( ; q != NULL; q = strpbrk(p, delims) )
          {
              output.push_back(typename C::value_type(p, q));
              p = q + 1;
          }

          // Insert last element
          if (p < &(s.back()) + 1) {
              output.push_back(typename C::value_type(p));
          }
      }

      output.swap(ret);
  }

  /**
   * Temporal implementation for mismatch with 2 starts and 2 ends. It is not in STL for c++11, but it will in c++14.
   *
   * reference: Third version at http://en.cppreference.com/w/cpp/algorithm/mismatch#Possible_implementation
   */
  template<class InputIt1, class InputIt2>
  std::pair<InputIt1, InputIt2>
  mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
  {
      while (first1 != last1 && first2 != last2 && *first1 == *first2) {
          ++first1, ++first2;
      }
      return std::make_pair(first1, first2);
  }

  inline std::string remove_end_of_line(std::string &line)
  {
      bool has_r = false, has_n = false;
      if (line.back() == '\n') {
          has_n = true;
          line.pop_back();
      }
      if (line.back() == '\r') {
          has_r = true;
          line.pop_back();
      }

      std::string eol;
      if (has_r) {
          eol.push_back('\r');
      }
      if (has_n) {
          eol.push_back('\n');
      }
      return eol;
  }

  inline bool is_remote_url(const std::string& input)
  {
      return boost::starts_with(input, util::HTTP) ||
             boost::starts_with(input, util::HTTPS) ||
             boost::starts_with(input, util::FTP) ||
             boost::starts_with(input, util::FTPS);
  }

}

#endif // UTIL_STRING_UTILS_HPP

