#pragma once

#include <algorithm>
#include <array>
#include <compare>
#include <cstdint>
#include <limits>
#include <endian.h>
#include <format>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

#include <ip/export_ipv4.hpp>

namespace ip {

struct IP_SHARED_EXPORT ipv4_address {
  using uint_t = uint32_t;
  using byte_t = uint8_t;
  using bytes_t = std::array<uint_t, 4>;

private:
  union in_addr_t {
    struct {
#if LITTLE_ENDIAN // 0xddccbbaa
      byte_t d, c, b, a;
#elif BIG_ENDIAN // 0xaabbccdd
      byte_t a, b, c, d;
#else
#error noendian
#endif
    } octet;
    uint_t u_addr;
  } m_in_addr{};

public:
  // ctors
  constexpr ipv4_address() noexcept : ipv4_address(0) {}
  constexpr ipv4_address(uint_t u) noexcept { m_in_addr.u_addr = u; }
  constexpr ipv4_address(bytes_t bs) noexcept : ipv4_address(bs[0], bs[1], bs[2], bs[3]) {}
  constexpr ipv4_address(byte_t a, byte_t b, byte_t c, byte_t d) noexcept
      : ipv4_address(static_cast<uint_t>(a << 24 | b << 16 | c << 8 | d)) {}
  explicit ipv4_address(const std::string_view &s) : ipv4_address(s, true) {}
  ipv4_address(const std::string_view &s, bool validate);

  // defaults
  // constexpr ipv4_address(const ipv4_address &a) noexcept = default;
  // ipv4_address(ipv4_address &&) noexcept = default;
  // ipv4_address &operator=(const ipv4_address &a) noexcept = default;
  // ipv4_address &operator=(ipv4_address &&) noexcept = default;

  // convertions
  constexpr uint_t as_uint() const noexcept { return m_in_addr.u_addr; }
  constexpr bytes_t as_bytes() const noexcept {
    return bytes_t{m_in_addr.octet.a, m_in_addr.octet.b, m_in_addr.octet.c, m_in_addr.octet.d};
  }
  constexpr std::string as_string() const noexcept {
    return std::format("{}.{}.{}.{}", m_in_addr.octet.a, m_in_addr.octet.b, m_in_addr.octet.c,
                       m_in_addr.octet.d); // to cache or not to cache...
  }

  // refs
  auto &at(size_t n) {
    switch (n) {
    case 0:
      return m_in_addr.octet.a;
    case 1:
      return m_in_addr.octet.b;
    case 2:
      return m_in_addr.octet.c;
    case 3:
      return m_in_addr.octet.d;
    }
    throw std::invalid_argument("Index out of range");
  }
  auto ref_octets() noexcept {
    return std::tie(m_in_addr.octet.a, m_in_addr.octet.b, m_in_addr.octet.c, m_in_addr.octet.d);
  }
  template <size_t N> ipv4_address& with(std::array<byte_t, N> const &bs) {
    for(size_t i=0; i < N; ++i)
      this->at(i) = bs[i];
    return *this;
  }  

  // compares
  friend constexpr auto operator<=>(const ipv4_address &l, const ipv4_address &r) noexcept {
    return l.m_in_addr.u_addr <=> r.m_in_addr.u_addr;
  } // for == return error: no match for «operator==» (operand types are «ip4» {aka «ip::ipv4_address»} and «ip4» {aka
    // «ip::ipv4_address»})
  // that's why operator== is added here explicitly
  friend constexpr bool operator==(const ipv4_address &lhs, const ipv4_address &rhs) noexcept {
    return lhs.m_in_addr.u_addr == rhs.m_in_addr.u_addr;
  }

  // soft equals
  template <size_t N> bool is_soft_equal(std::array<byte_t, N> const &rhs) const noexcept {
    auto lhs = as_bytes();
    return std::equal(lhs.cbegin(), lhs.cbegin() + std::min(lhs.size(), N), rhs.cbegin());
  }
  bool is_equal_any_of(std::set<ip::ipv4_address::byte_t> const &rhs) const noexcept {
    auto bs = as_bytes();
    std::set<ip::ipv4_address::byte_t> lhs(bs.begin(), bs.end());
    return std::any_of(lhs.begin(), lhs.end(), // = inline bool - no constexpr... )
                       [&rhs](const auto &b) { return rhs.contains(b); });
  }

  // friends
  friend std::ostream &operator<<(std::ostream &sout, const ipv4_address &a) { return sout << a.as_string(); }

  // statics
  static bool is_valid_format(std::string_view s) noexcept;
  static constexpr auto lowest() noexcept { return ipv4_address(0); }
  static constexpr auto highest() noexcept { return ipv4_address(std::numeric_limits<uint_t>::max()); } // = UINT32_MAX
  template <size_t N, typename compare_t> static auto range_with(std::array<byte_t, N> const &bs, compare_t comp = std::less<ipv4_address>()) {
    auto low = lowest().with(bs);
    auto up = highest().with(bs);
    if constexpr(comp(lowest(), highest()))
      return std::make_pair(low, up);
    else
      return std::make_pair(up, low);
  }

};

inline IP_SHARED_EXPORT ipv4_address::byte_t operator"" _b(unsigned long long ull) {
  return static_cast<ipv4_address::byte_t>(ull);
}

inline IP_SHARED_EXPORT ipv4_address operator"" _4i(unsigned long long ull) {
  return {static_cast<ipv4_address::uint_t>(ull)};
}

inline IP_SHARED_EXPORT ipv4_address operator"" _4s(const char *s, [[maybe_unused]] size_t size) {
  return ipv4_address(s);
}

} // namespace ip
