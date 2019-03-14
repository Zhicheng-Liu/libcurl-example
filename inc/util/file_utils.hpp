#ifndef UTIL_FILE_UTILS_HPP
#define UTIL_FILE_UTILS_HPP

#include <fstream>
#include <iostream>
#include <string>

#include <boost/algorithm/string/predicate.hpp>

#include "util/curl_easy.hpp"
#include "util/string_utils.hpp"

namespace util
{

  inline void open_file(std::ifstream & input,
                        std::string path,
                        std::ios_base::openmode mode = std::ios_base::in)
  {
      input.open(path,mode);
      if (!input) {
          std::string file_error_msg = "Couldn't open file " + path;
          throw std::runtime_error{file_error_msg};
      }
  }

  inline std::ostream & open_remote(std::ostream & stream, const std::string & url)
  {
      if (!is_remote_url(url)) {
          std::string msg = "The URL is incorrect or the protocol is not supported: ";
          msg += url;
          throw std::invalid_argument{msg};
      }

      curl::Easy curl;
      return curl.request(stream, url);
  }

}

#endif // UTIL_FILE_UTILS_HPP
