#include <regex>
#include <ip/ipv4_address.hpp>

// ipv4_address = dec-octet "." dec-octet "." dec-octet "." dec-octet
// dec-octet   = DIGIT                 ; 0-9
//             / %x31-39 DIGIT         ; 10-99
//             / "1" 2DIGIT            ; 100-199
//             / "2" %x30-34 DIGIT     ; 200-249
//             / "25" %x30-35          ; 250-255
ip::ipv4_address::ipv4_address(const std::string_view &s, bool validate) {
  if (validate & !is_valid_format(s)) 
    throw std::invalid_argument(std::format("Invalid ip format: {}", s));
  sscanf(s.data(), "%hhu.%hhu.%hhu.%hhu", 
          &m_in_addr.octet.a, 
          &m_in_addr.octet.b, 
          &m_in_addr.octet.c,
          &m_in_addr.octet.d);    
}

bool ip::ipv4_address::is_valid_format(std::string_view s) noexcept {
  static const std::regex r(
      "^(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$",
      std::regex_constants::ECMAScript);
  return std::regex_match(s.data(), r);
}
