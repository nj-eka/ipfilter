#pragma once
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

#include "ipv4_address.hpp"

namespace ip {

using vip_t = std::vector<ipv4_address>;

inline auto output_iterator(std::ostream &sout = std::cout){
  return std::ostream_iterator<ipv4_address>(sout, "\n");
}

inline IP_SHARED_EXPORT void print_all(const vip_t &vip, std::ostream &sout) 
{
  std::copy(vip.begin(), vip.end(), output_iterator(sout));
}

template <typename compare_t, typename... Args> inline IP_SHARED_EXPORT void print_if(
  const vip_t &vip, std::ostream &sout, compare_t comp, Args &&...args)
{
  const std::array<ipv4_address::byte_t, sizeof...(Args)> bs = {std::forward<Args>(args)...};
  if constexpr(std::random_access_iterator<decltype(vip.begin())>){
    auto [lower, upper] = ipv4_address::range_with(bs, comp);
    if (auto from = std::lower_bound(vip.begin(), vip.end(), lower, comp); from != vip.end()){
      std::copy(
        from,
        // case when num matches is less than num of steps in binary search
        //std::find_if_not(from, vip.end(), [&upper, comp](const auto& addr) { return comp(addr, upper); }),
        std::upper_bound(from, vip.end(), upper, comp),
        output_iterator(sout)
      );
    };
  } else {
    auto is_soft_equal = [&bs](const ip::ipv4_address& addr) { return addr.is_soft_equal(bs); };
    if (auto first = std::find_if(vip.begin(), vip.end(), is_soft_equal); first != vip.end()){
      std::copy(
        first,
        std::find_if_not(first, vip.end(), is_soft_equal), 
        output_iterator(sout)
      );
    };
  };
}

template <typename... Args> inline IP_SHARED_EXPORT void print_if_any(const vip_t &vip, std::ostream &sout, Args &&...args) 
{
  const std::set<ipv4_address::byte_t> sb = {std::forward<Args>(args)...};
  std::copy_if(vip.begin(), vip.end(), 
    output_iterator(sout),
    [&sb](const auto& addr){ return addr.is_equal_any_of(sb); }
  );
}

} // namespace ip