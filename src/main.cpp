#include "args/options.hpp"
#include "io/redirection.hpp"
#include <ip/ipv4_address.hpp>
#include <ip/ipv4_filter.hpp>

int main(int argc, char const *argv[]) {
  try {
    auto [desc, vm] = args::parse(argc, argv);
    if (args::info_requested(vm, desc)){
      return EAGAIN;
    }
    ip::vip_t vip;
    vip.reserve(vm["counts"].as<size_t>());
    // ios_base::sync_with_stdio(false);
    {
      const std::unique_ptr<io::istream2file_redirect> in_guard =
          vm.count("input") ? std::make_unique<io::istream2file_redirect>(vm["input"].as<std::string>()) : nullptr;
      for (std::string line; std::getline(std::cin, line);) {
        size_t pos = line.find_first_of('\t');
        if (pos != std::string::npos && pos > 0) {
          vip.emplace_back(line.substr(0, pos));
        } else
          throw std::invalid_argument(std::format("Invalid line format [{}]", line));
      }
      vip.shrink_to_fit();
    }

    const std::unique_ptr<io::ostream2file_redirect> out_guard =
        vm.count("output") ? std::make_unique<io::ostream2file_redirect>(vm["output"].as<std::string>()) : nullptr;

    // TODO reverse lexicographically sort
    auto comp = std::greater<ip::ipv4_address>();
    std::sort(vip.begin(), vip.end(), comp);
    ip::print_all(vip, std::cout);

    using ip::operator""_b;
    // TODO filter by first byte and output
    ip::print_if(vip, std::cout, comp, 1_b);

    // TODO filter by first and second bytes and output
    ip::print_if(vip, std::cout, comp, 46_b, 70_b);

    // TODO filter by any byte and output
    ip::print_if_any(vip, std::cout, 46_b);

  } catch (const args::opt::error &e) {
    std::cerr << "Invalid command args / ERROR: " << e.what() << " (see --help)" << std::endl;
    return EINVAL;
  } catch (const std::system_error &e) {
    std::cerr << e.what() << " (" << e.code() << ")" << std::endl;
    return e.code().value();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return ENOEXEC;
  }
  return 0;
}
