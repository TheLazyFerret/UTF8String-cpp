//! Inner representation of each UTF-8 symbol.
//! This header is also included by utf8_string.hpp, but can be used standalone.

#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
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
  constexpr explicit UTF8Symbol(char ascii_char);                        // Constructor for ASCII characters.
  ~UTF8Symbol() = default;                                               // Destructor.
  constexpr UTF8Symbol(const UTF8Symbol&) = default;                     // Copy constructor.
  constexpr UTF8Symbol& operator=(const UTF8Symbol&) = default;          // Copy assignment.
  constexpr UTF8Symbol(UTF8Symbol&&) = default;                          // Move constructor.
  constexpr UTF8Symbol& operator=(UTF8Symbol&&) = default;               // Move assignment.

  std::string to_string() const noexcept;
  constexpr char to_char() const;

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

} // namespace utf8_string
