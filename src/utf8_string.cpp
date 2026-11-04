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

/// Return a std::string with the bytes of each symbol hold.
/// It needs to loop all symbols, and all bytes of all symbols, so it is not a cheap conversion.
/// It should never fail, unless allocation error or stuff like that.
///
/// Use a relative conservative ammount of bytes reserved, so most UTF8Strings should only need one allocation.
std::string UTF8String::to_string() const {
  std::string result;
  if (inner_data.size() == 0) {
    return result; // Directly return an empty string.
  }
  // Reserve a conservative ammount of memory.
  result.reserve(inner_data.size() * 2);
  for (const auto& symbol : inner_data) {
    const auto symbol_data = symbol.data();
    const char* aux = reinterpret_cast<const char*>(symbol_data.first.data());
    result.insert(result.end(), aux, aux + symbol_data.second);
  }
  return result;
}
