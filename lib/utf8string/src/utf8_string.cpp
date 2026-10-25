//! Source code of the UTF8String.

#include "utf8_string.hpp"
#include "utf8_symbol.hpp"

#include <array>
#include <cstring>

using namespace utf8_string;

/// Return a std::string with the bytes of each symbol hold.
/// It needs to loop all symbols, and all bytes of all symbols, so it is not a cheap conversion.
/// It should never fail, unless allocation error or stuff like that.
std::string UTF8String::to_string() const noexcept {
  std::string result;
  if (inner_data.size() == 0) {
    return result; // Directly return an empty string.
  }
  // Reserve the worst case scenario.
  result.reserve(inner_data.size() * UTF8_SYMBOL_SIZE);
  for (const auto& symbol : inner_data) {
    auto symbol_data = symbol.data();
    const auto array_size = symbol_data.second;
    std::array<char, 4> array_aux;
    // A memcpy probably is the fastest way to do a byte reconversion.
    std::memcpy(array_aux.data(), symbol_data.first.data(), array_aux.size());
    result.insert(result.end(), array_aux.begin(), array_aux.begin() + array_size);
  }
  result.shrink_to_fit();
  return result;
}
