#include <arpa/inet.h>
#include <cstring>
#include <gtest/gtest.h>

#include <ip/ipv4_address.hpp>

using ip4 = ip::ipv4_address;

TEST(ipv4_address_test, sizeof) { EXPECT_EQ(sizeof(ip4), sizeof(ip4::uint_t)); }

TEST(ipv4_address_test, inet_aton_compliance) {
  auto s = "1.2.3.4";
  struct in_addr addr;
  if (inet_aton(s, &addr) == 0) {
    throw std::invalid_argument(std::format("Invalid ip format: {} /error: {}", s, strerror(errno)));
  }
  EXPECT_EQ(ntohl(addr.s_addr), ip4(s));
}

TEST(ipv4_address_test, string_validation) {
  EXPECT_ANY_THROW(ip4("trololo"));
  EXPECT_ANY_THROW(ip4("1"));
  EXPECT_ANY_THROW(ip4(".1"));
  EXPECT_ANY_THROW(ip4("1.1"));
  EXPECT_ANY_THROW(ip4(".1.1"));
  EXPECT_ANY_THROW(ip4("1.1.1"));
  EXPECT_ANY_THROW(ip4(".1.1.1"));
  EXPECT_ANY_THROW(ip4(".1.1.1.1"));
  EXPECT_ANY_THROW(ip4("1."));
  EXPECT_ANY_THROW(ip4("1.1."));
  EXPECT_ANY_THROW(ip4("1.1.1."));
  EXPECT_ANY_THROW(ip4("01.0.0.0"));
  EXPECT_ANY_THROW(ip4("0.01.0.0"));
  EXPECT_ANY_THROW(ip4("0.0.01.0"));
  EXPECT_ANY_THROW(ip4("0.0.0.01"));
  EXPECT_ANY_THROW(ip4("000001.000000.00100.010"));
  EXPECT_ANY_THROW(ip4("0.0.0.-1"));
  EXPECT_ANY_THROW(ip4("0.0.-1.0"));
  EXPECT_ANY_THROW(ip4("0.-1.0.0"));
  EXPECT_ANY_THROW(ip4("-1.0.0.0"));
  EXPECT_ANY_THROW(ip4("0.0.0.256"));
  EXPECT_ANY_THROW(ip4("0.0.256.0"));
  EXPECT_ANY_THROW(ip4("0.256.0.0"));
  EXPECT_ANY_THROW(ip4("256.0.0.0"));
}

TEST(ipv4_address_test, ctor_string) {
  EXPECT_EQ(ip4("0.0.0.0").as_uint(), 0);
  EXPECT_EQ(ip4("0.0.0.1").as_uint(), 1);
  EXPECT_EQ(ip4("0.0.0.255").as_uint(), 255);
  EXPECT_EQ(ip4("0.0.1.0").as_uint(), 1 << 8);
  EXPECT_EQ(ip4("0.0.1.1").as_uint(), 1 << 8 | 1);
  EXPECT_EQ(ip4("0.0.255.255").as_uint(), 255 << 8 | 255);
  EXPECT_EQ(ip4("0.2.0.0").as_uint(), 2 << 16);
  EXPECT_EQ(ip4("0.2.0.4").as_uint(), 2 << 16 | 4);
  EXPECT_EQ(ip4("0.2.3.4").as_uint(), 2 << 16 | 3 << 8 | 4);
  EXPECT_EQ(ip4("2.3.4.5").as_uint(), 2 << 24 | 3 << 16 | 4 << 8 | 5);
  EXPECT_EQ(ip4("255.255.255.255").as_uint(), 255 << 24 | 255 << 16 | 255 << 8 | 255);
}

TEST(ipv4_address_test, ctor_int) {
  EXPECT_EQ(ip4(0).as_uint(), 0);
  EXPECT_EQ(ip4(2 << 24 | 3 << 16 | 4 << 8 | 5).as_uint(), 2 << 24 | 3 << 16 | 4 << 8 | 5);
  EXPECT_EQ(ip4(255 << 24 | 255 << 16 | 255 << 8 | 255).as_uint(), 255 << 24 | 255 << 16 | 255 << 8 | 255);
  // EXPECT_ANY_THROW(ip4(-1));
  // EXPECT_ANY_THROW(ip4(1213.4));
}

TEST(ipv4_address_test, ctor_bytes) {
  EXPECT_EQ(ip4(0, 0, 0, 0).as_uint(), 0);
  EXPECT_EQ(ip4(1, 2, 3, 4).as_uint(), 1 << 24 | 2 << 16 | 3 << 8 | 4);

  // ошибка: unsigned conversion from «int» to «ip::ipv4_address::byte_t» {aka «unsigned char»} changes value from «256»
  // to «0» [-Werror=overflow] EXPECT_ANY_THROW(ip4(256,0,0,0)); 
  // EXPECT_ANY_THROW(ip4(-1,0,0,0));    - Actually it doesn't... todo: implicit conversion problem should be resolved? if yes then how?...
}

TEST(ipv4_address_test, as_bytes) {
  auto bs = ip4("1.11.111.255").as_bytes();
  EXPECT_EQ(bs.size(), 4);
  EXPECT_EQ(bs[0], 1);
  EXPECT_EQ(bs[1], 11);
  EXPECT_EQ(bs[2], 111);
  EXPECT_EQ(bs[3], 255);
}

TEST(ipv4_address_test, as_string) {
  auto s = "1.2.3.4";
  EXPECT_STREQ(s, ip4(s).as_string().c_str());
  EXPECT_STREQ(s, ip4(ip4(s).as_uint()).as_string().c_str());
}

TEST(ipv4_address_test, comparing) {
  EXPECT_TRUE(ip4("1.0.0.0") > ip4("0.255.255.255"));
  EXPECT_TRUE(ip4("0.1.0.0") > ip4("0.0.255.255"));
  EXPECT_TRUE(ip4("0.0.1.0") > ip4("0.0.0.255"));
  EXPECT_TRUE(ip4("0.0.0.1") > ip4("0.0.0.0"));

  EXPECT_FALSE(ip4("1.0.0.0") < ip4("0.255.255.255"));
  EXPECT_FALSE(ip4("0.1.0.0") < ip4("0.0.255.255"));
  EXPECT_FALSE(ip4("0.0.1.0") < ip4("0.0.0.255"));
  EXPECT_FALSE(ip4("0.0.0.1") < ip4("0.0.0.0"));

  EXPECT_TRUE(ip4("0.0.0.100") > ip4("0.0.0.99"));
  EXPECT_TRUE(ip4("0.0.100.0") > ip4("0.0.99.255"));
  EXPECT_TRUE(ip4("0.100.0.0") > ip4("0.99.255.255"));
  EXPECT_TRUE(ip4("100.0.0.0") > ip4("99.255.255.255"));

  EXPECT_TRUE(ip4("100.0.0.0") == ip4(100, 0, 0, 0));
  EXPECT_TRUE(ip4("100.0.0.0") >= ip4(100, 0, 0, 0));
  EXPECT_TRUE(ip4("100.0.0.0") <= ip4(100, 0, 0, 0));
  EXPECT_TRUE(ip4("100.0.0.0") > ip4(99, 255, 255, 255));
  EXPECT_TRUE(ip4("99.255.255.255") < ip4(100, 0, 0, 0));
}

TEST(ipv4_address_test, comparing_uint) {
  EXPECT_TRUE(ip4("0.0.0.100") == 100);
  EXPECT_TRUE(100 == ip4("0.0.0.100"));
  EXPECT_TRUE(0xFF00FF00 == ip4("255.0.255.0"));
  EXPECT_TRUE(ip4(255, 0, 255, 0) == 0b11111111000000001111111100000000);
}

TEST(ipv4_address_test, at) {
  EXPECT_TRUE(ip4(1, 2, 3, 4).at(0) == 0b00000001);
  ip4 a{1, 2, 3, 4};
  a.at(0) = 111;
  a.at(1) = 11;
  a.at(2) = 1;
  a.at(3) = 0;
  EXPECT_TRUE(a == ip4(111, 11, 1, 0));
  EXPECT_ANY_THROW(a.at(4));
  EXPECT_ANY_THROW(a.at(-1));
}

TEST(ipv4_address_test, octets) {
  ip4 cip{1, 2, 3, 4};
  auto [a, b, c, d] = cip.ref_octets();
  a = 111;
  b = 11;
  c = 1;
  d = 0;
  EXPECT_TRUE(cip == ip4("111.11.1.0"));
}

TEST(ipv4_address_test, print) {
  auto s = "11.22.33.44";
  std::stringstream ss;
  ss << ip4(s);
  EXPECT_STREQ(s, ss.str().c_str());
}

TEST(ipv4_address_test, literal) {
  using ip::operator""_4s;
  using ip::operator""_4i;
  using ip::operator""_b;

  EXPECT_EQ("11.22.33.44"_4s.at(2), 33);
  EXPECT_EQ(0xFF00FF00_4i .at(2), 255);
  EXPECT_EQ(0xFF00FF00_4i, "255.0.255.0"_4s);
}
