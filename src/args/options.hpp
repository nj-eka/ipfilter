#include <boost/program_options.hpp>
#include <fstream>
#include <info/version.h>
#include <iostream>
#include <string>
#include <tuple>
namespace args {

namespace opt = boost::program_options;

constexpr const size_t COUNTS = 1000;

inline auto parse(int argc, char const *argv[]) {
  opt::options_description desc("Usage");
  // clang-format off
  desc.add_options()
    ("help,h",      "Print this help message")
    ("version,v",   "Print version")
    ("info",        "Print project info")
    ("input,i",     opt::value<std::string>(), "Input file (tsv) path [default: stdin]")
    ("output,o",    opt::value<std::string>(), "Output file path [default: stdout]")
    ("counts,c",    opt::value<size_t>()->default_value(COUNTS), "Initial size to reserve")  
  ;
  // clang-format on
  opt::positional_options_description pos_desc;
  pos_desc.add("input", -1);
  opt::variables_map vm;
  opt::store(opt::command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), vm);
  opt::notify(vm);
  return std::make_tuple(std::move(desc), std::move(vm));
}

inline bool info_requested(const opt::variables_map &vm, const opt::options_description &desc) {
  if (vm.count("help")) {
    std::cout << desc << std::endl;
    return true;
  }
  if (vm.count("version")) {
    std::cout << info::info_project_version() << std::endl;
    return true;
  }
  if (vm.count("info")) {
    std::cout << info::info_project_full() << std::endl;
    return true;
  }
  return false;
}

} // namespace args
