//! Source code of the UTF8String.

#include "utf8_string.hpp"
#include "utf8_symbol.hpp"

#include <iostream>
#include <string>

using namespace utf8_string;

/// Outputs in the stream the data.
std::ostream& utf8_string::operator<<(std::ostream& os, const UTF8String& str) {
  for (const auto& symbol : str.inner_data) {
    os << symbol;
  }
  return os;
}

/// Retrieve a UTF8String from output.
std::istream& utf8_string::operator>>(std::istream& is, UTF8String& str) {
  // Use a std::string. Not need to reinvent the wheel.
  std::string aux;
  is >> aux;
  str = UTF8String(aux);
  return is;
}
