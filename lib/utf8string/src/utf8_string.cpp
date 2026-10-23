//! Source code of the UTF8String.

#include "utf8_string.hpp"
#include "utf8_symbol.hpp"

#include <stdexcept>

using namespace utf8_string;

/// Construct a new UTF8String from a str.
/// It only does at much a allocation for holding the resulting value.
UTF8String::UTF8String(std::string_view str) {
  // Reserve the worst case memory needed to hold the converted string.
  inner_data.reserve(str.size());
  for (size_t i = 0; i < str.size(); ++i) {
    const auto size_result = bytes_of_utf8(str[i]);
    // If fails, it is because the parameter is bad encoded.
    if (!size_result) {
      throw std::invalid_argument(std::string(size_result.error()));
    }
    const std::string_view slice = str.substr(i, size_result.value());
    inner_data.emplace_back(slice);
    i += size_result.value() - 1; // Need to substract 1 because skips 1 byte otherwise.
  }
}

/// Return a std::string with the bytes of each symbol hold.
/// It needs to loop all symbols, and all bytes of all symbols, so it is not a cheap conversion.
/// It should never fail, unless allocation error or stuff like that.
std::string UTF8String::to_string() const noexcept {
  std::string result;
  if (inner_data.size() == 0) {
    return result; // Directly return an empty string.
  }
  result.reserve(inner_data.size() * UTF8_SYMBOL_SIZE);
  for (const auto& symbol : inner_data) {
    result += symbol.to_string();
  }
  return result;
}
