//! Private implementations not shared to the public interface.

#pragma once

#include <array>
#include <expected>
#include <string_view>

namespace utf8_string::impl {

/// Example unicode symbol.
[[maybe_unused]] constexpr std::string_view EXAMPLE_UNICODE = "ඞ";

constexpr std::array<char8_t, 4> F_BYTE_MASK = {0b10000000, 0b11100000, 0b11110000, 0b11111000};
constexpr std::array<char8_t, 4> F_BYTE_RESULT = {0b00000000, 0b11000000, 0b11100000, 0b11110000};
constexpr char8_t NF_BYTE_MASK = 0b11000000;
constexpr char8_t NF_BYTE_RESULT = 0b10000000;

/// Test a byte, ANDing the mask and comparing with the expected result.
constexpr bool test_8byte(char8_t byte, char8_t mask, char8_t result) noexcept { return (byte & mask) == result; }

/// Test the byte (pressumable the first in a UTF-8 symbol) and return the number of
/// bytes the symbol is composed.
constexpr std::expected<size_t, std::string_view> bytes_of_utf8(char byte) noexcept {
  constexpr std::string_view ERROR_MESSAGE = "Not encoded UTF8 byte.";
  const char8_t aux = static_cast<char8_t>(byte);
  if (test_8byte(aux, F_BYTE_MASK[0], F_BYTE_RESULT[0])) {
    return 1;
  } else if (test_8byte(aux, F_BYTE_MASK[1], F_BYTE_MASK[1])) {
    return 2;
  } else if (test_8byte(aux, F_BYTE_MASK[2], F_BYTE_MASK[2])) {
    return 3;
  } else if (test_8byte(aux, F_BYTE_MASK[3], F_BYTE_MASK[3])) {
    return 4;
  }
  return std::unexpected(ERROR_MESSAGE);
}

} // namespace utf8_string::impl
