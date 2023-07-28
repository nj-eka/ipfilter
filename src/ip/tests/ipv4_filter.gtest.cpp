#include <gtest/gtest.h>

#include <ip/ipv4_filter.hpp>

// using ip::operator""_4s;
using ip::operator""_4i;
using ip::operator""_b;

using ip4 = ip::ipv4_address;

using vip_t = std::vector<ip4>;

TEST(ipv4_filter_test, soft_equals) {
  vip_t vip = {{0, 1, 1, 2}, {1, 1, 0, 0}, {1, 1, 1, 0}, {1, 1, 2, 3}, {1, 2, 3, 4}, {1, 9, 10, 11}};
  auto comp = std::less<ip4>();
  {
    std::stringstream ss;
    ip::print_if(vip, ss, comp, 0_b);
    EXPECT_EQ(ss.str(), "0.1.1.2\n");
  }

  {
    std::stringstream ss;
    ip::print_if(vip, ss, comp, 1_b, 1_b);
    EXPECT_EQ(ss.str(), "1.1.0.0\n1.1.1.0\n1.1.2.3\n");
  }

  {
    std::stringstream ss;
    ip::print_if(vip, ss, comp, 1_b, 1_b, 1_b);
    EXPECT_EQ(ss.str(), "1.1.1.0\n");
  }

}

TEST(ipv4_filter_test, any_of) {
  vip_t vip = {ip4("0.1.1.1"),     ip4("2.3.4.5"),    ip4("6.7.8.9"),
                   ip4("10.11.12.13"), ip4("255.0.0.11"), ip4("3.1.0.0")};

  {
    std::stringstream ss;
    ip::print_if_any(vip, ss, 1_b);
    EXPECT_EQ(ss.str(), "0.1.1.1\n3.1.0.0\n");
  }

  {
    std::stringstream ss;
    ip::print_if_any(vip, ss, 255_b, 11_b);
    EXPECT_EQ(ss.str(), "10.11.12.13\n255.0.0.11\n");
  }
}
