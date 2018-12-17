#include "util/curl_easy.hpp"

size_t
StringWriterCallback(void *contents, size_t size, size_t nmemb, void *buffer) {
  ((std::string*)buffer)->append((const char*)contents, size * nmemb);
  return size * nmemb;
}

size_t
StreamWriterCallback(void *contents, size_t size, size_t nmemb, void *stream) {
  ((std::ostream*)stream)->write((const char*)contents, static_cast<std::streamsize>(size * nmemb));
  return size * nmemb;
}

curl::Easy::Easy() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curlHandle = curl_easy_init();
  if ( curlHandle ) {
    curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 1L); // TODO: remove this
//    curl_easy_setopt(curlHandle, CURLOPT_DNS_SERVERS, "8.8.8.8");
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curlHandle, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curlHandle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
  }
}

curl::Easy::~Easy() {
  if ( curlHandle ) {
    curl_easy_cleanup(curlHandle);
  }
  curl_global_cleanup();
}

std::string
curl::Easy::request(const std::string &url) {
  std::string buffer;
  if ( curlHandle ) {
    curl_easy_setopt(curlHandle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, StringWriterCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &buffer);
    CURLcode res = curl_easy_perform(curlHandle);
    if (res != CURLE_OK) {
      std::cerr << "curl::Easy::request failed: " << url << std::endl;
      std::cerr << "Reason: " << curl_easy_strerror(res) << std::endl;
    }
  }
  return buffer;
}

std::ostream&
curl::Easy::request(std::ostream& stream, const std::string& url) {
  if ( curlHandle ) {
    curl_easy_setopt(curlHandle, CURLOPT_URL, curl_easy_escape(curlHandle, url.c_str(), url.size()));
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, StreamWriterCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &stream);
    CURLcode res = curl_easy_perform(curlHandle);
    if (res != CURLE_OK) {
      std::cerr << "curl::Easy::request failed: " << url << std::endl;
      std::cerr << "Reason: " << curl_easy_strerror(res) << std::endl;
      char *seturl = NULL;
      curl_easy_getinfo(curlHandle, CURLINFO_EFFECTIVE_URL, &seturl);
      if(seturl)
        printf("Redirect to: %s\n", seturl);
    }
  }
  return stream;
}
