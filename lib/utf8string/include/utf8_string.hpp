//! My personal implementation for working with UTF-8 strings in c++.
//! I decided to use with a vector because this way I don't need to worry about
//! manually manage the memory.

#pragma once

#include "utf8_symbol.hpp"

#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace utf8_string {

class UTF8String {
public:
  constexpr UTF8String() noexcept = default;                    // Default constructor.
  constexpr explicit UTF8String(std::string_view);              // Constructor from string_view.
  ~UTF8String() = default;                                      // Destructor.
  constexpr UTF8String(const UTF8String&) = default;            // Copy constructor.
  constexpr UTF8String& operator=(const UTF8String&) = default; // Copy assignment.
  constexpr UTF8String(UTF8String&&) = default;                 // Move constructor.
  constexpr UTF8String& operator=(UTF8String&&) = default;      // Move assignment.

  constexpr std::string to_string() const noexcept;

private:
  std::vector<UTF8Symbol> inner_data;
};

/// Construct a new UTF8String from a str.
/// It only does at much a allocation for holding the resulting value.
constexpr UTF8String::UTF8String(std::string_view str) {
  // Reserve the worst case memory needed to hold the converted string.
  inner_data.reserve(str.size());
  for (size_t i = 0; i < str.size(); ++i) {
    const auto size = bytes_of_utf8(str[i]);
    // If fails, it is because the parameter is bad encoded.
    if (size <= 0) {
      throw std::invalid_argument("Bad encoded symbol.");
    }
    const std::string_view slice = str.substr(i, size);
    inner_data.emplace_back(slice);
    i += size - 1; // Need to substract 1 because skips 1 byte otherwise.
  }
}

/// Return a std::string with the bytes of each symbol hold.
/// It needs to loop all symbols, and all bytes of all symbols, so it is not a cheap conversion.
/// It should never fail, unless allocation error or stuff like that.
constexpr std::string UTF8String::to_string() const noexcept {
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

} // namespace utf8_string
