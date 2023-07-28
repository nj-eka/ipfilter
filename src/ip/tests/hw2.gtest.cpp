#include <fstream>
#include <gtest/gtest.h>
#include <hash/md5.hpp>       //instead of <openssl/md5.h> | <boost/md5.hpp>

#include <ip/ipv4_filter.hpp>

#define HW2_TEST_FILE "ip_filter.tsv"

using namespace ip;

using vip_t = std::vector<ipv4_address>;

TEST(ipv4_filter_test, hmd5) {
  ccat::hash::md5 m{"hello world"};
  ASSERT_STREQ(m.hexdigest().c_str(), "5eb63bbbe01eeed093cb22bb8f5acdc3");
}

TEST(ipv4_filter_test, hw2) {
  vip_t vip;
  {  
    auto file_deleter = [](std::ifstream *__ptr) {
      if (__ptr->is_open())
        __ptr->close();
      delete __ptr;
    };
    // std::unique_ptr test_file{new std::ifstream(HW2_TEST_FILE), file_deleter};    doesn't work
    std::unique_ptr<std::ifstream, decltype(file_deleter)> test_file{new std::ifstream(HW2_TEST_FILE), file_deleter};
    ASSERT_TRUE(test_file->is_open());
    for (std::string line; std::getline(*test_file, line);) {
      ASSERT_NO_THROW(vip.emplace_back(line.substr(0, line.find_first_of('\t')))) << "Invalid line format: " << line;
    }
  }
  vip.shrink_to_fit();
  auto comp = std::greater<ipv4_address>();
  std::sort(vip.begin(), vip.end(), comp);
  std::stringstream ss;
  ip::print_all(vip, ss);
  ip::print_if(vip, ss, comp, 1_b);
  ip::print_if(vip, ss, comp, 46_b, 70_b);
  ip::print_if_any(vip, ss, 46_b);
  ASSERT_STREQ(ccat::hash::md5{ss.str()}.hexdigest().c_str(), "24e7a7b2270daee89c64d3ca5fb3da1a");
}
