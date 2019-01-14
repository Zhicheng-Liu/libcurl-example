//
// Created by Zhicheng Liu on 2018-12-17.
//
#include <fstream>
#include <sstream>
#include <string>

#include "catch/catch.hpp"
#include "util/curl_easy.hpp"

TEST_CASE()
{
  SECTION("test download")
  {
    std::string url = "https://www.ebi.ac.uk/ena/data/view/AB325691&display=fasta";
    curl::Easy curl;
    std::stringstream ss;
    curl.request(ss, url);
    std::ifstream data{"test/data/AB325691.fa"};
    std::stringstream sd;
    while(sd << data.rdbuf());
    CHECK(ss.str() == sd.str());
  }
}
