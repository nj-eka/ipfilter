#include <cerrno>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

namespace io {

class istream2file_redirect {
  std::istream &_to;
  std::ifstream _from;
  std::streambuf *_saved;

public:
  istream2file_redirect(const std::string &filename, std::istream &to = std::cin)
      : _to{to}, _from{filename}, _saved{nullptr} {
    // from.exceptions(from.failbit | from.badbit);
    if (!_from.is_open())
      throw std::system_error(errno ? errno : EIO, std::system_category(),
                              std::format("Failed to open file [{}]", filename));
    _saved = _to.rdbuf(
        _from.rdbuf()); // in this case 'saved' should NOT be initialized in a member initializer of the constructor
  }

  istream2file_redirect(const istream2file_redirect &) = delete;
  void operator=(const istream2file_redirect &) = delete;
  ~istream2file_redirect() {
    if (_saved)
      _to.rdbuf(_saved);
  }
};

class ostream2file_redirect {
  std::ostream &_from;
  std::ofstream _to;
  std::streambuf *_saved;

public:
  ostream2file_redirect(const std::string &filename, std::ostream &from = std::cout)
      : _from{from}, _to{filename, std::ios_base::trunc | std::ios_base::out}, _saved{nullptr} {
    // to.exceptions(to.failbit | to.badbit);
    if (!_to.is_open())
      throw std::system_error(errno ? errno : EIO, std::system_category(),
                              std::format("Failed to open file [{}]", filename));
    _saved = _from.rdbuf(_to.rdbuf());
  }
  ostream2file_redirect(const ostream2file_redirect &) = delete;
  void operator=(const ostream2file_redirect &) = delete;
  ~ostream2file_redirect() {
    if (_saved)
      _from.rdbuf(_saved);
  }
};

} // namespace io