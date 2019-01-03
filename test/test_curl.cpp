//
// Created by Zhicheng Liu on 2018-12-17.
//
#include <fstream>
#include <sstream>
#include <string>

#include <boost/filesystem.hpp>

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
    std::cout << "*********************" << std::endl;
    std::cout << ss.str() << std::endl;
    std::cout << "*********************" << std::endl;
    CHECK(ss.str() == sd.str());
  }
}
