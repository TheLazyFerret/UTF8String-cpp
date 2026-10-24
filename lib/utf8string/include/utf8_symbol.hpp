//! Inner representation of each UTF-8 symbol.
//! This header is also included by utf8_string.hpp, but can be used standalone.
//! One of my objetives is also to be as much constexpr and allocation free as I can.
//! Also, it is guaranteed that always will point to a valid UTF8 character.

#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace utf8_string {

/// The size used for storing a single UTF8 symbol.
constexpr size_t UTF8_SYMBOL_SIZE = 4;

constexpr std::array<char8_t, 4> F_BYTE_MASK = {0b10000000, 0b11100000, 0b11110000, 0b11111000};
constexpr std::array<char8_t, 4> F_BYTE_RESULT = {0b00000000, 0b11000000, 0b11100000, 0b11110000};
constexpr char8_t NF_BYTE_MASK = 0b11000000;
constexpr char8_t NF_BYTE_RESULT = 0b10000000;

/// Test a byte, ANDing the mask and comparing with the expected result.
constexpr bool test_8byte(char8_t byte, char8_t mask, char8_t result) noexcept { return (byte & mask) == result; }

/// Test the byte (pressumable the first in a UTF-8 symbol) and return the number of
/// bytes the symbol is composed.
constexpr short bytes_of_utf8(char byte) noexcept {
  const char8_t aux = static_cast<char8_t>(byte);
  if (test_8byte(aux, F_BYTE_MASK[0], F_BYTE_RESULT[0])) {
    return 1;
  } else if (test_8byte(aux, F_BYTE_MASK[1], F_BYTE_RESULT[1])) {
    return 2;
  } else if (test_8byte(aux, F_BYTE_MASK[2], F_BYTE_RESULT[2])) {
    return 3;
  } else if (test_8byte(aux, F_BYTE_MASK[3], F_BYTE_RESULT[3])) {
    return 4;
  }
  return -1;
}

/// Represents a single UTF-8 symbol.
/// UTF-8 has a variable length, something that is hard to implement at compile time.
/// Due doing tons of allocations for the sake of saving a few bytes is very unnecesary,
/// decided to go with an array of 4 bytes.
class UTF8Symbol {
public:
  constexpr UTF8Symbol() = default;
  /// Build an UTF8Symbol from a string view containing a single UTF8 encoded character.
  /// It is also important to note that is not possible build '\0' from this constructor.
  /// If you need '\0' use the default empty constructor, or build from character.
  constexpr explicit UTF8Symbol(std::string_view str) { *this = decode_str(str); }
  constexpr explicit UTF8Symbol(char ascii_char);
  constexpr ~UTF8Symbol() = default;
  constexpr UTF8Symbol(const UTF8Symbol&) = default;
  constexpr UTF8Symbol& operator=(const UTF8Symbol&) = default;
  constexpr UTF8Symbol(UTF8Symbol&&) = default;
  constexpr UTF8Symbol& operator=(UTF8Symbol&&) = default;

  constexpr char to_char() const;
  constexpr std::uint32_t to_integer() const noexcept;
  constexpr size_t byte_size() const noexcept { return static_cast<size_t>(size); }

  std::string to_string() const noexcept;
  constexpr std::pair<std::array<char8_t, UTF8_SYMBOL_SIZE>, size_t> data() const noexcept;
  std::vector<char8_t> data_vec() const noexcept;

  friend std::ostream& operator<<(std::ostream&, const UTF8Symbol&);
  friend std::istream& operator>>(std::istream&, UTF8Symbol&) noexcept;

  constexpr friend bool operator==(const UTF8Symbol&, const UTF8Symbol&) noexcept;
  constexpr friend bool operator!=(const UTF8Symbol&, const UTF8Symbol&) noexcept;
  constexpr friend bool operator<(const UTF8Symbol&, const UTF8Symbol&) noexcept;

private:
  constexpr static UTF8Symbol decode_str(std::string_view str);

  // Attributes. By default it is equivalent to the '\0' ASCII character.
  std::array<char8_t, UTF8_SYMBOL_SIZE> inner_data = {'\0', '\0', '\0', '\0'};
  unsigned char size = 1;
};

/// Constexpr constructor fot ASCII char.
/// It can throw an exception if the char is negative.
constexpr UTF8Symbol::UTF8Symbol(char ascii_char) {
  if (ascii_char < 0) {
    throw std::invalid_argument("The ASCII character has a not valid value.");
  }
  inner_data.front() = ascii_char;
  size = 1;
}

/// Return as a pair the inner data of the Symbol. Uses a pair so it doesn't allocate anything.
constexpr std::pair<std::array<char8_t, UTF8_SYMBOL_SIZE>, size_t> UTF8Symbol::data() const noexcept {
  return std::make_pair(inner_data, static_cast<size_t>(size));
}

/// Return the inner value as a normal c-char.
/// Throws an exception if the UTF-8 symbol is bigger than 1 byte (Not ASCII).
constexpr char UTF8Symbol::to_char() const {
  if (size > 1) {
    throw std::runtime_error("Invalid conversion to ASCII char from UTF-8.");
  }
  return inner_data.front();
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

/// Do the numeric comparison for lhs < rhs.
constexpr bool operator<(const UTF8Symbol& lhs, const UTF8Symbol& rhs) noexcept {
  // Comparable due the UTF8Symbol always has the exact bit size.
  // Never a more significant byte will be 0.
  if (lhs.size < rhs.size) {
    return true;
  } else if (lhs.size > rhs.size) {
    return false;
  }
  // Both has the same size. Therefore need to compare the number.
  return lhs.to_integer() < rhs.to_integer();
}

#define CHECK_SIZE(size, expected_size, msg)                                                                           \
  if (size != expected_size) {                                                                                         \
    throw std::invalid_argument(msg);                                                                                  \
  }

/// Byte parser for an UTF-8 codified unicode symbol.
/// It is allocation free.
/// It is important that the str only has one UTF8 symbol. Will throw otherwise.
constexpr UTF8Symbol UTF8Symbol::decode_str(std::string_view str) {
  const char* const ERROR_MESSAGE = "The raw str is not a UTF-8 symbol.";
  if (!(str.size() >= 1 && str.size() <= 4)) {
    throw std::invalid_argument(ERROR_MESSAGE);
  }
  UTF8Symbol result;
  const char8_t first_char = static_cast<char8_t>(str.front());
  // The unicode symbol is two byte long.
  if (test_8byte(first_char, F_BYTE_MASK[0], F_BYTE_RESULT[0])) {
    CHECK_SIZE(str.size(), 1, ERROR_MESSAGE)
    result.inner_data = {first_char, '\0', '\0', '\0'};
    result.size = 1;
    return result;
  }
  // The unicode symbol is two byte long.
  else if (test_8byte(first_char, F_BYTE_MASK[1], F_BYTE_RESULT[1])) {
    CHECK_SIZE(str.size(), 2, ERROR_MESSAGE)
    const char8_t second_char = static_cast<char8_t>(str.at(1));
    if (!test_8byte(second_char, NF_BYTE_MASK, NF_BYTE_RESULT)) {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    result.inner_data = {first_char, second_char, '\0', '\0'};
    result.size = 2;
    return result;
  }
  // The unicode symbol is thre byte long.
  else if (test_8byte(first_char, F_BYTE_MASK[2], F_BYTE_RESULT[2])) {
    CHECK_SIZE(str.size(), 3, ERROR_MESSAGE)
    const char8_t second_char = static_cast<char8_t>(str.at(1));
    const char8_t third_char = static_cast<char8_t>(str.at(2));
    if (!test_8byte(second_char, NF_BYTE_MASK, NF_BYTE_RESULT) ||
        !test_8byte(third_char, NF_BYTE_MASK, NF_BYTE_RESULT)) {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    result.inner_data = {first_char, second_char, third_char, '\0'};
    result.size = 3;
    return result;
  }
  // The unicode symbol is four byte long.
  else if (test_8byte(first_char, F_BYTE_MASK[3], F_BYTE_RESULT[3])) {
    CHECK_SIZE(str.size(), 4, ERROR_MESSAGE)
    const char8_t second_char = static_cast<char8_t>(str.at(1));
    const char8_t third_char = static_cast<char8_t>(str.at(2));
    const char8_t fourth_char = static_cast<char8_t>(str.at(3));
    if (!test_8byte(second_char, NF_BYTE_MASK, NF_BYTE_RESULT) ||
        !test_8byte(third_char, NF_BYTE_MASK, NF_BYTE_RESULT) ||
        !test_8byte(fourth_char, NF_BYTE_MASK, NF_BYTE_RESULT)) {
      throw std::invalid_argument(ERROR_MESSAGE);
    }
    result.inner_data = {first_char, second_char, third_char, fourth_char};
    result.size = 4;
    return result;
  }
  // The first byte is bad coded.
  throw std::invalid_argument(ERROR_MESSAGE);
}

#undef CHECK_SIZE

constexpr std::uint32_t UTF8Symbol::to_integer() const noexcept {
  assert(size > 0 && size <= 4);
  std::uint32_t value = 0;
  for (size_t i = 0; i < size; ++i) {
    // Move one byte to left.
    value = value << 8;
    // Add the next byte.
    value = (value | static_cast<std::uint32_t>(inner_data[i]));
  }
  return value;
}

} // namespace utf8_string

/// Struct for allowing use std::format (and also std::print or std::println).
template <> struct std::formatter<utf8_string::UTF8Symbol> {
  /// Parse the options of the formatter (what is inside the {}).
  auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

  /// Return the formatted value of the UTF8Symbol.
  /// Actually returns an Iterator.
  constexpr auto format(const utf8_string::UTF8Symbol& sym, std::format_context& ctx) const {
    return std::format_to(ctx.out(), "{}", sym.to_string());
  }

  [[maybe_unused]] bool is_hex = false;
  [[maybe_unused]] bool is_bin = false;
};
