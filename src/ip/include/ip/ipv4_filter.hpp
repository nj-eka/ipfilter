#pragma once
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>
#include <type_traits>
#include <concepts>

#include "ipv4_address.hpp"

namespace ip {

template<typename It>
concept Ipv4InIterator = std::is_base_of_v<std::input_iterator_tag, typename std::iterator_traits<It>::iterator_category> &&
  std::is_same_v<ipv4_address, typename std::iterator_traits<It>::value_type>;

template<typename T>
concept Ipv4InIterable = requires(T t){
  {t.begin()} -> Ipv4InIterator<>; // ++(t.begin()); *(t.begin());
  {t.begin() == t.end()} -> std::convertible_to<bool>;
  {t.begin() != t.end()} -> std::convertible_to<bool>;
};

inline auto output_iterator(std::ostream &sout = std::cout){
  return std::ostream_iterator<ipv4_address>(sout, "\n");
}

template<Ipv4InIterable ipv4_container_t>
IP_SHARED_EXPORT void print_all(const ipv4_container_t &cont, std::ostream &out) 
{
  std::copy(cont.begin(), cont.end(), output_iterator(out));
}

template<Ipv4InIterable ipv4_container_t, typename compare_t, typename... Args> 
IP_SHARED_EXPORT void print_if(
  const ipv4_container_t &cont, std::ostream &out, compare_t comp, Args &&...args)
{
  const std::array<ipv4_address::byte_t, sizeof...(Args)> bs = {std::forward<Args>(args)...};
  if constexpr(std::random_access_iterator<decltype(cont.begin())>){
    auto [lower, upper] = ipv4_address::range_with(bs, comp);
    if (auto from = std::lower_bound(cont.begin(), cont.end(), lower, comp); from != cont.end()){
      std::copy(
        from,
        // case when num matches is less than num of steps in binary search
        //std::find_if_not(from, vip.end(), [&upper, comp](const auto& addr) { return comp(addr, upper); }),
        std::upper_bound(from, cont.end(), upper, comp),
        output_iterator(out)
      );
    };
  } else {
    auto is_soft_equal = [&bs](const ip::ipv4_address& addr) { return addr.is_soft_equal(bs); };
    if (auto first = std::find_if(cont.begin(), cont.end(), is_soft_equal); first != cont.end()){
      std::copy(
        first,
        std::find_if_not(first, cont.end(), is_soft_equal), 
        output_iterator(out)
      );
    };
  };
}

template <Ipv4InIterable ipv4_container_t, typename... Args> 
IP_SHARED_EXPORT void print_if_any(const ipv4_container_t &cont, std::ostream &out, Args &&...args) 
{
  const std::set<ipv4_address::byte_t> sb = {std::forward<Args>(args)...};
  std::copy_if(cont.begin(), cont.end(), 
    output_iterator(out),
    [&sb](const auto& addr){ return addr.is_equal_any_of(sb); }
  );
}

} // namespace ip
