#ifndef CURL_EASY_HPP
#define CURL_EASY_HPP

#include <iostream>
#include <string>

#include <curl/curl.h>

namespace curl
{
  class Easy final
  {
  public:
    Easy();
    Easy(const Easy&) = delete;
    Easy& operator=(const Easy&) = delete;
    ~Easy();

    std::string request(const std::string& url);
    std::ostream& request(std::ostream& stream, const std::string& url);

  private:
    CURL* curlHandle;
  };
}

#endif //CURL_EASY_HPP
