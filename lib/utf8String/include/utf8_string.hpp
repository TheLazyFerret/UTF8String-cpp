//! My personal implementation for working with UTF-8 strings in c++.

#pragma once

#include <array>
#include <cstddef>
#include <string_view>

namespace utf8_string {

constexpr size_t UTF8_SYMBOL_SIZE = 4;
constexpr std::array<char8_t, UTF8_SYMBOL_SIZE> EMPTY_UTF8_SYMBOL = {'\0', '\0', '\0', '\0'};

/// Represents a single UTF-8 symbol.
/// UTF-8 has a variable length, something that is hard to implement at compile time.
/// Due doing tons of allocations for the sake of saving a few bytes is very unnecesary,
/// decided to go with an array of 4 bytes.
class UTF8Symbol {
public:
  constexpr UTF8Symbol() : inner_data(EMPTY_UTF8_SYMBOL), size(1) {}     // Default constructor.
  explicit UTF8Symbol(std::string_view str) { *this = decode_str(str); } // Multi-byte string constructor.
  ~UTF8Symbol() = default;                                               // Destructor.
  constexpr UTF8Symbol(const UTF8Symbol&) = default;                     // Copy constructor.
  constexpr UTF8Symbol& operator=(const UTF8Symbol&) = default;          // Copy assignment.
  constexpr UTF8Symbol(UTF8Symbol&&) = default;                          // Move constructor.
  constexpr UTF8Symbol& operator=(UTF8Symbol&&) = default;               // Move assignment.

private:
  static UTF8Symbol decode_str(std::string_view str);

  // Attributes.
  std::array<char8_t, UTF8_SYMBOL_SIZE> inner_data;
  unsigned char size;
};

} // namespace utf8_string
