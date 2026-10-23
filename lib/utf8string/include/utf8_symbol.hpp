//! Inner representation of each UTF-8 symbol.
//! This header is also included by utf8_string.hpp, but can be used standalone.
//! One of my objetives is also to be as much constexpr and allocation free as I can.

#pragma once

#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace utf8_string {

/// The size used for storing a single UTF8 symbol.
constexpr size_t UTF8_SYMBOL_SIZE = 4;
// The default UTF8 symbol value. For the default constructor, size will be one, so will be equivalent to '\0'.
constexpr std::array<char8_t, UTF8_SYMBOL_SIZE> EMPTY_UTF8_SYMBOL = {'\0', '\0', '\0', '\0'};

/// Represents a single UTF-8 symbol.
/// UTF-8 has a variable length, something that is hard to implement at compile time.
/// Due doing tons of allocations for the sake of saving a few bytes is very unnecesary,
/// decided to go with an array of 4 bytes.
class UTF8Symbol {
public:
  constexpr UTF8Symbol() : inner_data(EMPTY_UTF8_SYMBOL), size(1) {}     // Default constructor.
  explicit UTF8Symbol(std::string_view str) { *this = decode_str(str); } // Multi-byte string constructor.
  constexpr explicit UTF8Symbol(char ascii_char);                        // Constructor for ASCII characters.
  ~UTF8Symbol() = default;                                               // Destructor.
  constexpr UTF8Symbol(const UTF8Symbol&) = default;                     // Copy constructor.
  constexpr UTF8Symbol& operator=(const UTF8Symbol&) = default;          // Copy assignment.
  constexpr UTF8Symbol(UTF8Symbol&&) = default;                          // Move constructor.
  constexpr UTF8Symbol& operator=(UTF8Symbol&&) = default;               // Move assignment.

  std::string to_string() const noexcept;
  constexpr char to_char() const;

  constexpr size_t byte_size() const noexcept { return static_cast<size_t>(size); }
  constexpr std::pair<std::array<char8_t, UTF8_SYMBOL_SIZE>, size_t> data() const noexcept;

  friend std::ostream& operator<<(std::ostream&, const UTF8Symbol&);
  friend std::istream& operator>>(std::istream&, UTF8Symbol&) noexcept;

  constexpr friend bool operator==(const UTF8Symbol&, const UTF8Symbol&) noexcept;
  constexpr friend bool operator!=(const UTF8Symbol&, const UTF8Symbol&) noexcept;

private:
  static UTF8Symbol decode_str(std::string_view str);

  // Attributes.
  std::array<char8_t, UTF8_SYMBOL_SIZE> inner_data;
  unsigned char size;
};

/// Constexpr constructor fot ASCII char.
/// It can throw an exception if the char is negative ()
constexpr UTF8Symbol::UTF8Symbol(char ascii_char) {
  if (ascii_char < 0) {
    throw std::invalid_argument("The ASCII character has a not valid value.");
  }
  inner_data.front() = ascii_char;
  size = 1;
}

/// Return the inner value as a normal c-char.
/// Throws an exception if the UTF-8 symbol is bigger than 1 byte (Not ASCII).
constexpr char UTF8Symbol::to_char() const {
  if (size > 1) {
    throw std::runtime_error("Invalid conversion to ASCII char from UTF-8.");
  }
  return inner_data.front();
}

/// Return as a pair the inner data of the Symbol. Uses a pair so it doesn't allocate anything.
constexpr std::pair<std::array<char8_t, UTF8_SYMBOL_SIZE>, size_t> UTF8Symbol::data() const noexcept {
  return std::make_pair(inner_data, static_cast<size_t>(size));
}

/// Compare if two UTF8 symbols are equal.
constexpr bool operator==(const UTF8Symbol& lhs, const UTF8Symbol& rhs) noexcept {
  if (lhs.size != rhs.size) {
    return false;
  }
  for (size_t i = 0; i < lhs.size; ++i) {
    if (lhs.inner_data[i] != rhs.inner_data[i]) {
      return false;
    }
  }
  return true;
}

/// Compare if two UTF8 are not equal.
constexpr bool operator!=(const UTF8Symbol& lhs, const UTF8Symbol& rhs) noexcept { return !(lhs == rhs); }

} // namespace utf8_string

/// Struct for allowing use std::format (and also std::print or std::println).
template <> struct std::formatter<utf8_string::UTF8Symbol> {
  /// Parse the options of the formatter (what is inside the {}).
  constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  /// Return the formatted value of the UTF8Symbol.
  /// Actually returns an Iterator.
  auto format(const utf8_string::UTF8Symbol& sym, std::format_context& ctx) const {
    return std::format_to(ctx.out(), "{}", sym.to_string());
  }

  [[maybe_unused]] bool is_hex = false;
  [[maybe_unused]] bool is_bin = false;
};
