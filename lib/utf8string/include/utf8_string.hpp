//! My personal implementation for working with UTF-8 strings in c++.
//! I decided to use with a vector because this way I don't need to worry about
//! manually manage the memory.

#pragma once

#include "utf8_symbol.hpp"

#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <stdexcept>


namespace utf8_string {

class UTF8String {
public:
  constexpr UTF8String() noexcept = default;
  constexpr explicit UTF8String(std::string_view);
  constexpr ~UTF8String() = default;
  constexpr UTF8String(const UTF8String&) = default;
  constexpr UTF8String& operator=(const UTF8String&) = default;
  constexpr UTF8String(UTF8String&&) = default;
  constexpr UTF8String& operator=(UTF8String&&) = default;

  std::string to_string() const noexcept;

  constexpr size_t size() const noexcept { return inner_data.size(); }
  constexpr void push_back(const UTF8Symbol& symbol) noexcept { inner_data.push_back(symbol); }
  constexpr void push_back(UTF8Symbol&& symbol) noexcept { inner_data.push_back(std::move(symbol)); }

private:
  std::vector<UTF8Symbol> inner_data;
};

/// Construct a new UTF8String from a str.
/// It only does as much one allocation for holding the resulting value.
constexpr UTF8String::UTF8String(std::string_view str) {
  // Reserve the worst case memory needed to hold the converted string.
  inner_data.reserve(str.size());
  for (size_t i = 0; i < str.size(); ++i) {
    const auto size = bytes_of_utf8(str[i]);
    // If fails, it is because the parameter is bad encoded.
    if (size <= 0) {
      throw std::invalid_argument("Bad encoded symbol.");
    }
    const std::string_view slice = str.substr(i, static_cast<size_t>(size));
    inner_data.emplace_back(slice);
    i += static_cast<size_t>(size) - 1; // Need to substract 1 because skips 1 byte otherwise.
  }
  inner_data.shrink_to_fit();
}

} // namespace utf8_string
