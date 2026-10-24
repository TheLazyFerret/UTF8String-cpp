//! Source code of the UTF8String.

#include "utf8_string.hpp"
#include "utf8_symbol.hpp"

using namespace utf8_string;

/// Return a std::string with the bytes of each symbol hold.
/// It needs to loop all symbols, and all bytes of all symbols, so it is not a cheap conversion.
/// It should never fail, unless allocation error or stuff like that.
std::string UTF8String::to_string() const noexcept {
  std::string result;
  if (inner_data.size() == 0) {
    return result; // Directly return an empty string.
  }
  // Reserve the worst possible sceneario memory.
  result.reserve(inner_data.size() * UTF8_SYMBOL_SIZE);
  for (const auto& symbol : inner_data) {
    result += symbol.to_string();
  }
  result.shrink_to_fit();
  return result;
}
